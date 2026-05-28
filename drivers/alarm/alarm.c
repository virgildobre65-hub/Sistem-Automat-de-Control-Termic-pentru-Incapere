#include "alarm.h"
#include <avr/io.h>

void Alarm_Init(void) {
    DDRB |= (1 << PB1);
    PORTB &= ~(1 << PB1);
}

void Alarm_Update(float current_temp, float cold_threshold) {
    if (current_temp < cold_threshold) {
        PORTB |= (1 << PB1);
    } else {
        PORTB &= ~(1 << PB1);
    }
}