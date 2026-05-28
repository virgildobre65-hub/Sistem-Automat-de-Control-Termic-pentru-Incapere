#include <stdio.h>
#include <string.h>
#include <math.h>

#include "drivers/adc/adc.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"

#include "drivers/usart/usart.h"
#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"

#define POT_ADC_CHANNEL           1
#define THERMISTOR_ADC_CHANNEL    0
#define LOG_INTERVAL_MS           1000UL

typedef enum {
    MODE_SET_FAN   = 0, 
    MODE_SET_ALARM = 1 
} SetMode_t;

const float bCoefficient = 3950.0f;

float calculate_temp(uint16_t adc_value) {
    if (adc_value < 10) return 100.0f; 
    float resistance = 10000.0f / (1023.0f / (float)adc_value - 1.0f);
    float steinhart = log(resistance / 10000.0f) / bCoefficient;
    steinhart += 1.0f / (25.0f + 273.15f);
    return (1.0f / steinhart) - 273.15f;
}

int main(void) {
    ADC_Init();
    Timer0_Init();
    USART_Init(16000000UL, 9600);
    
    GPIO_Init(D9, GPIO_OUTPUT);
    GPIO_Init(D10, GPIO_OUTPUT); 
    GPIO_Init(D2, GPIO_INPUT);
    
    // Activare Pull-up pentru buton folosind registrul direct sau functia GPIO (D2 corespunde PORTD, Pin 2)
    PORTD |= (1 << PD2); 
    
    // Initializeaza PWM pe Pinul D11 (Port B, Pin 3) la frecventa de 25kHz
    PWM_Init(GPIO_PORTB, 3, 25000);

    SetMode_t set_mode = MODE_SET_FAN;
    float fan_threshold = 25.0f;
    float cold_threshold = 15.0f;
    
    uint32_t last_log = 0;
    uint32_t last_button_time = 0;
    uint8_t last_button_state = 0;
    uint32_t last_blink = 0;
    
    char msg[128];
    char* alert_msg = "\r\n--- MODIFICARE COMUTATA ---\r\n";
    char* clear_msg = "\033[H\033[J";
    char* sep_msg = "------------------------------------\r\n";

    while (1) {
        uint32_t current_time = Millis();
        float current_temp = calculate_temp(ADC_Read(THERMISTOR_ADC_CHANNEL)); 
        uint16_t pot_val = ADC_Read(POT_ADC_CHANNEL);                  

        // --- CITIRE BUTON SI DEBOUNCE NON-BLOCKING ---
        uint8_t current_button_state = !(PIND & (1 << PD2)); 
        if (current_button_state && !last_button_state && (current_time - last_button_time >= 300)) {
            set_mode = (set_mode == MODE_SET_FAN) ? MODE_SET_ALARM : MODE_SET_FAN;
            USART_Transmit(alert_msg, (uint8_t)strlen(alert_msg));
            last_button_time = current_time;
        }
        last_button_state = current_button_state;

        // --- REGLAJ INDIVIDUAL IN FUNCTIE DE MOD ---
        float pot_ratio = (float)pot_val / 1023.0f;
        if (set_mode == MODE_SET_FAN) {
            fan_threshold = 18.0f + (pot_ratio * 17.0f);
        } else {
            cold_threshold = 10.0f + (pot_ratio * 25.0f);
        }

        // --- CONTROL VENTILATOR (PWM prin Driver) ---
        if (current_temp >= fan_threshold) {
            PWM_SetDutyCycle(GPIO_PORTB, 3, 255); 
        } else if (current_temp < (fan_threshold - 1.0f)) {
            PWM_SetDutyCycle(GPIO_PORTB, 3, 0);
        }

        // --- CONTROL LED ALARMA (Blink Non-blocking prin Driver GPIO) ---
        if (current_temp < cold_threshold) {
            if (current_time - last_blink > 300) {
                GPIO_Toggle(D9);
                last_blink = current_time;
            }
        } else {
            GPIO_Write(D9, GPIO_LOW);
        }

        // --- MONITORIZARE SERIALA (Functii din usart.h) ---
        if (current_time - last_log >= LOG_INTERVAL_MS) {
            USART_Transmit(clear_msg, (uint8_t)strlen(clear_msg)); 

            sprintf(msg, "FAN -> Setat: %dC | Actual: %dC %s\r\n", 
                    (int)fan_threshold, (int)current_temp, 
                    (set_mode == MODE_SET_FAN ? "<< REGLAJ ACTIV" : " "));
            USART_Transmit(msg, (uint8_t)strlen(msg));

            sprintf(msg, "ALR -> Setata: %dC | Actuala: %dC %s\r\n", 
                    (int)cold_threshold, (int)current_temp, 
                    (set_mode == MODE_SET_ALARM ? "<< REGLAJ ACTIV" : " "));
            USART_Transmit(msg, (uint8_t)strlen(msg));
            
            USART_Transmit(sep_msg, (uint8_t)strlen(sep_msg));
            last_log = current_time;
        }
    }
    return 0;
}