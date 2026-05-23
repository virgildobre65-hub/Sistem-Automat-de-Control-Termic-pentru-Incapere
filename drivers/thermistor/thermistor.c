#include "thermistor.h"

#include <math.h>

#include "drivers/adc/adc.h"

float Thermistor_GetTemperature(uint16_t adc_value) {
    /* Protecție împotriva valorilor ADC invalide */
    if (adc_value < 10) {
        return 100.0f; /* Termistor deconectat sau scurtcircuit */
    }

    /* Calcul rezistență termistor din divizorul de tensiune */
    float resistance = THERMISTOR_R_SERIES /
                       (1023.0f / (float)adc_value - 1.0f);

    float steinhart = logf(resistance / THERMISTOR_R_NOMINAL) / THERMISTOR_B_COEFF;
    steinhart += 1.0f / (THERMISTOR_T_NOMINAL + 273.15f);

    return (1.0f / steinhart) - 273.15f;
}

float Thermistor_Read(void) {
    uint16_t raw = ADC_Read(THERMISTOR_ADC_CHANNEL);
    return Thermistor_GetTemperature(raw);
}