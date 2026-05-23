#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>

/**
 * @brief Inițializează LED-ul de alarmă (pinul D9).
 */
void Alarm_Init(void);

/**
 * @brief Actualizează starea LED-ului de alarmă în funcție de temperatură.
 *
 * Dacă temperatura este sub prag, LED-ul clipește la 300 ms.
 * Dacă temperatura este la sau peste prag, LED-ul rămâne stins.
 *
 * @param current_temp  Temperatura curentă măsurată (°C)
 * @param threshold     Pragul de alarmă de frig (°C)
 */
void Alarm_Update(float current_temp, float threshold);

#endif /* ALARM_H */
