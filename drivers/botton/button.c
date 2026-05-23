#include "button.h"

#include <avr/io.h>
#include <util/delay.h>

#include "drivers/gpio/gpio.h"

/** Debounce delay (ms) */
#define BUTTON_DEBOUNCE_MS  200

void Button_Init(void) {
    GPIO_Init(D2, GPIO_INPUT);
    PORTD |= (1 << PD2); /* Activare pull-up intern */
}

uint8_t Button_IsPressed(void) {
    static uint8_t prev_state = 0;

    /* Butonul este activ LOW (pull-up activat) */
    uint8_t current_state = !(PIND & (1 << PD2));

    if (current_state && !prev_state) {
        /* Front crescător detectat */
        prev_state = current_state;
        _delay_ms(BUTTON_DEBOUNCE_MS); /* Debounce simplu */
        return 1;
    }

    prev_state = current_state;
    return 0;
}