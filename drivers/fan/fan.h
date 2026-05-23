#ifndef FAN_H
#define FAN_H

#include <stdint.h>

/**
 * @brief Inițializează PWM-ul pentru controlul ventilatorului.
 *
 * Configurează pinul D10 (PB2) ca ieșire PWM la 25 kHz.
 */
void Fan_Init(void);

/**
 * @brief Actualizează starea ventilatorului în funcție de temperatură.
 *
 * Ventilatorul pornește la 100% când temperatura >= prag.
 * Ventilatorul se oprește când temperatura < (prag - 1°C) (histerezis).
 *
 * @param current_temp  Temperatura curentă măsurată (°C)
 * @param threshold     Pragul de pornire al ventilatorului (°C)
 */
void Fan_Update(float current_temp, float threshold);

/**
 * @brief Oprește forțat ventilatorul (duty cycle 0).
 */
void Fan_Stop(void);

#endif /* FAN_H */