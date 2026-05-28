#include "button.h"
#include "nano.h"
#include <avr/io.h>

void Button_Init(void) {
    DDRD &= ~(1 << D2);
    PORTD |= (1 << D2);
}

uint8_t Button_IsPressed(void) {
    static uint8_t last_state = 1;
    uint8_t current_state = (PIND & (1 << D2)) ? 1 : 0;
    
    if (last_state == 1 && current_state == 0) {
        last_state = current_state;
        return 1;
    }
    
    last_state = current_state;
    return 0;
}