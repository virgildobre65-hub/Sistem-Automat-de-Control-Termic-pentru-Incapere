#include "alarm.h"

#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"

/** Intervalul de clipire al LED-ului de alarmă (ms) */
#define ALARM_BLINK_INTERVAL_MS  300UL

void Alarm_Init(void) {
    GPIO_Init(D9, GPIO_OUTPUT);
    GPIO_Write(D9, GPIO_LOW);
}

void Alarm_Update(float current_temp, float threshold) {
    if (current_temp < threshold) {
        /* Temperatura sub prag → LED clipește */
        static uint32_t last_blink = 0;
        uint32_t now = Millis();

        if (now - last_blink >= ALARM_BLINK_INTERVAL_MS) {
            GPIO_Toggle(D9);
            last_blink = now;
        }
    } else {
        /* Temperatura normală → LED stins */
        GPIO_Write(D9, GPIO_LOW);
    }
}
