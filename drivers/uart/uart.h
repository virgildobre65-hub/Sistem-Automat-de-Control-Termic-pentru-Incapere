#ifndef UART_H
#define UART_H
 
#include <stdint.h>
 
/**
 * @brief Inițializează modulul UART cu rata de baud specificată.
 * @param baud Rata de transmisie (ex: 9600)
 */
void UART_Init(uint32_t baud);
 
/**
 * @brief Trimite un șir de caractere prin UART.
 * @param s Pointer către șirul de caractere (terminat cu '\0')
 */
void UART_SendString(const char *s);
 
#endif /* UART_H */
 