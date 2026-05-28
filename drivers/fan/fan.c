#include "fan.h"
#include "../../drivers/pwm/pwm.h"
#include "../../bsp/nano.h"

#define FAN_PORT GPIO_PORTB
#define FAN_PIN 3

void Fan_Init(void) {
    PWM_Init(FAN_PORT, FAN_PIN, 1000);
    PWM_SetDutyCycle(FAN_PORT, FAN_PIN, 0);
}

void Fan_Update(float current_temp, float threshold) {
    if (current_temp > threshold) {
        float diff = current_temp - threshold;
        int duty = (int)(diff * 25.5f);
        
        if (duty > 255) duty = 255;
        if (duty < 60) duty = 60; 
        
        PWM_SetDutyCycle(FAN_PORT, FAN_PIN, (uint8_t)duty);
    } else {
        PWM_SetDutyCycle(FAN_PORT, FAN_PIN, 0);
    }
}