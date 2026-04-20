#include "uart.h"
#include <avr/io.h>
#include <stdio.h>

// Initialize standard IO stream for printf
static FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void uart_init(unsigned long baud) {
    uint16_t ubrr = F_CPU / 16 / baud - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable receiver and transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data format

    stdout = &uart_output; // Redirect stdout to UART
}

void uart_transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data; // Put data into buffer, sends the data
}

void uart_print(const char* str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_transmit('\r');
    }
    uart_transmit(c);
    return 0;
}
