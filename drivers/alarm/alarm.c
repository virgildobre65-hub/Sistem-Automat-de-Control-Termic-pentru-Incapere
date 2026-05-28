#include "alarm.h"
#include "nano.h"
#include <avr/io.h>

void Alarm_Init(void) {
    DDRB |= (1 << D9);
    PORTB &= ~(1 << D9);
}

void Alarm_Update(float current_temp, float cold_threshold) {
    if (current_temp < cold_threshold) {
        PORTB |= (1 << D9);
    } else {
        PORTB &= ~(1 << D9);
    }
}