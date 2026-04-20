#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(unsigned long baud);
void uart_transmit(unsigned char data);
void uart_print(const char* str);
// Define printf redirect for AVR
int uart_putchar(char c, FILE *stream);

#endif
