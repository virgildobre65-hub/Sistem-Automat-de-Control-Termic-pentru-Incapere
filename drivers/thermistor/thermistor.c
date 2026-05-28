#include "thermistor.h"
#include "../../drivers/adc/adc.h"
#include <math.h>

#define THERMISTOR_PIN 0
#define SERIES_RESISTOR 10000.0f
#define NOMINAL_RESISTANCE 10000.0f
#define NOMINAL_TEMPERATURE 25.0f
#define B_COEFFICIENT 3950.0f

float Thermistor_Read(void) {
    uint16_t adc_value = ADC_Read(THERMISTOR_PIN);
    
    if (adc_value == 0 || adc_value == 1023) {
        return 25.0f;
    }
    
    float resistance = SERIES_RESISTOR / ((1023.0f / (float)adc_value) - 1.0f);
    
    float steinhart;
    steinhart = resistance / NOMINAL_RESISTANCE;
    steinhart = log(steinhart);
    steinhart /= B_COEFFICIENT;
    steinhart += 1.0f / (NOMINAL_TEMPERATURE + 273.15f);
    steinhart = 1.0f / steinhart;
    steinhart -= 273.15f;
    
    return steinhart;
}