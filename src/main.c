#include <stdio.h>

#include "drivers/adc/adc.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"

#include "drivers/uart/uart.h"
#include "components/thermistor/thermistor.h"
#include "components/fan/fan.h"
#include "components/alarm/alarm.h"
#include "components/button/button.h"

#define DEFAULT_FAN_THRESHOLD     25.0f

#define DEFAULT_COLD_THRESHOLD    15.0f

#define POT_ADC_CHANNEL           1

#define LOG_INTERVAL_MS           1000UL

typedef enum {
    MODE_SET_FAN   = 0, 
    MODE_SET_ALARM = 1 
} SetMode_t;


int main(void) {
    ADC_Init();
    Timer0_Init();
    UART_Init(9600);

    Fan_Init();
    Alarm_Init();
    Button_Init();

    SetMode_t   set_mode        = MODE_SET_FAN;
    float       fan_threshold   = DEFAULT_FAN_THRESHOLD;
    float       cold_threshold  = DEFAULT_COLD_THRESHOLD;
    uint32_t    last_log        = 0;
    char        msg[128];

    while (1) {
        float    current_temp = Thermistor_Read();
        uint16_t pot_val      = ADC_Read(POT_ADC_CHANNEL);

        if (Button_IsPressed()) {
            set_mode = (set_mode == MODE_SET_FAN) ? MODE_SET_ALARM
                                                  : MODE_SET_FAN;
            UART_SendString("\r\n--- MODIFICARE COMUTATA ---\r\n");
        }

        if (set_mode == MODE_SET_FAN) {
            fan_threshold = 18.0f + ((float)pot_val / 1023.0f) * 17.0f;
        } else {
            cold_threshold = 10.0f + ((float)pot_val / 1023.0f) * 25.0f;
        }

        Fan_Update(current_temp, fan_threshold);
        Alarm_Update(current_temp, cold_threshold);

        if (Millis() - last_log >= LOG_INTERVAL_MS) {
            UART_SendString("\033[H\033[J"); 

            sprintf(msg, "FAN -> Setat: %dC | Actual: %dC %s\r\n",
                    (int)fan_threshold,
                    (int)current_temp,
                    (set_mode == MODE_SET_FAN ? "<< REGLAJ ACTIV" : " "));
            UART_SendString(msg);

            sprintf(msg, "ALR -> Setata: %dC | Actuala: %dC %s\r\n",
                    (int)cold_threshold,
                    (int)current_temp,
                    (set_mode == MODE_SET_ALARM ? "<< REGLAJ ACTIV" : " "));
            UART_SendString(msg);

            UART_SendString("------------------------------------\r\n");

            last_log = Millis();
        }
    }

    return 0;
}