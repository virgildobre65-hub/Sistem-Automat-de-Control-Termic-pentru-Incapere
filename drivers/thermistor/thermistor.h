#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <stdint.h>

/** Canalul ADC la care este conectat termistorul */
#define THERMISTOR_ADC_CHANNEL  0

/** Coeficientul B al termistorului NTC */
#define THERMISTOR_B_COEFF      3950.0f

/** Rezistența nominală a termistorului la 25°C (Ohm) */
#define THERMISTOR_R_NOMINAL    10000.0f

/** Rezistența de pull-down din divizorul de tensiune (Ohm) */
#define THERMISTOR_R_SERIES     10000.0f

/** Temperatura nominală de referință (°C) */
#define THERMISTOR_T_NOMINAL    25.0f

/**
 * @brief Calculează temperatura în grade Celsius din valoarea ADC.
 *
 * Folosește ecuația Steinhart-Hart simplificată (modelul B).
 *
 * @param adc_value Valoarea brută ADC (0–1023)
 * @return Temperatura calculată în °C
 */
float Thermistor_GetTemperature(uint16_t adc_value);

/**
 * @brief Citește direct temperatura de la canalul ADC configurat.
 * @return Temperatura curentă în °C
 */
float Thermistor_Read(void);

#endif /* THERMISTOR_H */
