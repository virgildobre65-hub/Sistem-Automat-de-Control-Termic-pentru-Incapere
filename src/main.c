#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "drivers/adc/adc.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include "drivers/usart/usart.h"
#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"

#define POT_ADC_CHANNEL           1
#define THERMISTOR_ADC_CHANNEL    0
#define LOG_INTERVAL_MS           1000UL
#define LDR_CHANNEL               2
#define ILUMINAT_1                D5
#define ILUMINAT_2                D6

typedef enum {
    MODE_SET_FAN   = 0, 
    MODE_SET_ALARM = 1,
    MODE_SET_LIGHT = 2 
} SetMode_t;

const float bCoefficient = 3950.0f;

float calculate_temp(uint16_t adc_value) {
    if (adc_value < 10) return 100.0f; 
    float resistance = 10000.0f / (1023.0f / (float)adc_value - 1.0f);
    float steinhart = log(resistance / 10000.0f) / bCoefficient;
    steinhart += 1.0f / (25.0f + 273.15f);
    return (1.0f / steinhart) - 273.15f;
}

void ActualizeazaIluminat(uint16_t prag_curent) {
    static uint32_t last_ldr_read = 0; 
    if (Millis() - last_ldr_read >= 200) {
        last_ldr_read = Millis();
        uint16_t nivel_lumina = ADC_Read(LDR_CHANNEL);
        if (nivel_lumina < prag_curent) {
            GPIO_Write(ILUMINAT_1, GPIO_HIGH);
            GPIO_Write(ILUMINAT_2, GPIO_HIGH);
        } else {
            GPIO_Write(ILUMINAT_1, GPIO_LOW);
            GPIO_Write(ILUMINAT_2, GPIO_LOW);
        }
    }
}

int main(void) {
    ADC_Init();
    Timer0_Init();
    USART_Init(16000000UL, 9600);
    
    GPIO_Init(D9, GPIO_OUTPUT);
    GPIO_Init(D10, GPIO_OUTPUT); 
    GPIO_Init(D2, GPIO_INPUT);
    GPIO_Init(ILUMINAT_1, GPIO_OUTPUT);
    GPIO_Init(ILUMINAT_2, GPIO_OUTPUT);
    
    PORTD |= (1 << 2); 
    PWM_Init(GPIO_PORTB, 3, 25000);

    SetMode_t set_mode = MODE_SET_FAN;
    float fan_threshold = 25.0f;
    float cold_threshold = 15.0f;
    uint16_t light_threshold = 400; 
    
    uint32_t last_log = 0;
    uint32_t last_blink = 0;
    
    uint8_t last_button_state = 0;
    uint32_t last_debounce_time = 0;

    uint16_t last_pot_val = ADC_Read(POT_ADC_CHANNEL);
    
    char msg[128];
    char* alert_msg = "\r\n--- MODIFICARE COMUTATA ---\r\n";
    char* clear_msg = "\033[H\033[J";
    char* sep_msg = "------------------------------------\r\n";

    while (1) {
        uint32_t current_time = Millis();
        
        ActualizeazaIluminat(light_threshold);
        float current_temp = calculate_temp(ADC_Read(THERMISTOR_ADC_CHANNEL)); 
        uint16_t current_pot_val = ADC_Read(POT_ADC_CHANNEL);                  

        uint8_t current_reading = !(PIND & (1 << 2)); 
        
        if (current_reading != last_button_state) {
            if ((current_time - last_debounce_time) > 50) {
                if (current_reading == 1) {
                    if (set_mode == MODE_SET_FAN) {
                        set_mode = MODE_SET_ALARM;
                    } else if (set_mode == MODE_SET_ALARM) {
                        set_mode = MODE_SET_LIGHT;
                    } else {
                        set_mode = MODE_SET_FAN;
                    }
                    USART_Transmit(alert_msg, (uint8_t)strlen(alert_msg));
                    last_pot_val = current_pot_val;
                }
                last_debounce_time = current_time;
                last_button_state = current_reading;
            }
        }

        int pot_diff = (int)current_pot_val - (int)last_pot_val;
        if (abs(pot_diff) > 5) {
            last_pot_val = current_pot_val; 
            float pot_ratio = (float)current_pot_val / 1023.0f;
            
            if (set_mode == MODE_SET_FAN) {
                fan_threshold = 18.0f + (pot_ratio * 17.0f);
            } else if (set_mode == MODE_SET_ALARM) {
                cold_threshold = 10.0f + (pot_ratio * 25.0f);
            } else if (set_mode == MODE_SET_LIGHT) {
                light_threshold = current_pot_val; 
            }
        }

        if (current_temp >= fan_threshold) {
            PWM_SetDutyCycle(GPIO_PORTB, 3, 255); 
        } else if (current_temp < (fan_threshold - 1.0f)) {
            PWM_SetDutyCycle(GPIO_PORTB, 3, 0);
        }

        if (current_temp < cold_threshold) {
            if (current_time - last_blink > 300) {
                GPIO_Toggle(D9);
                last_blink = current_time;
            }
        } else {
            GPIO_Write(D9, GPIO_LOW);
        }

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
            
            sprintf(msg, "LUM -> Setat: %d  | Actual: %d  %s\r\n", 
                    light_threshold, ADC_Read(LDR_CHANNEL),
                    (set_mode == MODE_SET_LIGHT ? "<< REGLAJ ACTIV" : " "));
            USART_Transmit(msg, (uint8_t)strlen(msg));
            
            USART_Transmit(sep_msg, (uint8_t)strlen(sep_msg));
            last_log = current_time;
        }
    }
    return 0;
}