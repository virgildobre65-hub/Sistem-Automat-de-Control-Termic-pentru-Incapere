#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

/**
 * @brief Inițializează butonul pe pinul D2 cu pull-up intern activ.
 */
void Button_Init(void);

/**
 * @brief Procesează starea butonului și returnează 1 la apăsare (front crescător).
 *
 * Funcția detectează tranziția 0→1 (rising edge) și include un debounce
 * software de 200 ms bazat pe delay.
 *
 * @return 1 dacă butonul tocmai a fost apăsat, 0 altfel
 */
uint8_t Button_IsPressed(void);

#endif /* BUTTON_H */
