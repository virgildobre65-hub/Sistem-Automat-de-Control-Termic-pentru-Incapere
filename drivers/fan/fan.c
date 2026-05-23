#include "fan.h"

#include "drivers/gpio/gpio.h"
#include "drivers/pwm/pwm.h"

/** Histerezis aplicat pragului de oprire (°C) */
#define FAN_HYSTERESIS  1.0f

/** Pinul GPIO folosit pentru PWM ventilator (PB3 = OC2A pe Nano) */
#define FAN_PWM_PORT    GPIO_PORTB
#define FAN_PWM_PIN     3

/** Frecvența PWM (Hz) */
#define FAN_PWM_FREQ    25000UL

void Fan_Init(void) {
    GPIO_Init(D10, GPIO_OUTPUT);
    PWM_Init(FAN_PWM_PORT, FAN_PWM_PIN, FAN_PWM_FREQ);
    PWM_SetDutyCycle(FAN_PWM_PORT, FAN_PWM_PIN, 0);
}

void Fan_Update(float current_temp, float threshold) {
    if (current_temp >= threshold) {
        /* Pornire ventilator la viteză maximă */
        PWM_SetDutyCycle(FAN_PWM_PORT, FAN_PWM_PIN, 255);
    } else if (current_temp < (threshold - FAN_HYSTERESIS)) {
        /* Oprire ventilator (cu histerezis pentru a evita oscilațiile) */
        PWM_SetDutyCycle(FAN_PWM_PORT, FAN_PWM_PIN, 0);
    }
    /* Zona de histerezis: starea rămâne neschimbată */
}

void Fan_Stop(void) {
    PWM_SetDutyCycle(FAN_PWM_PORT, FAN_PWM_PIN, 0);
}
