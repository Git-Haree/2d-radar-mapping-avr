#include "hcsr04.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint16_t pulse_start = 0;
volatile uint16_t pulse_end = 0;
volatile uint8_t edge_state = 0; // 0 = waiting for rising edge, 1 = waiting for falling edge
volatile uint16_t distance_cm = 0;

void hcsr04_init(void) {
    // Set TRIG as output, ECHO as input
    DDRB |= (1 << TRIG_PIN);
    DDRD &= ~(1 << ECHO_PIN);
    
    // Enable pull-up on ECHO
    PORTD |= (1 << ECHO_PIN);

    // Configure Timer1 for normal mode
    TCCR1A = 0;
    // Set prescaler to 8 (so 1 tick = 0.5 us at 16MHz)
    TCCR1B = (1 << CS11);

    // Configure INT0 for any logical change
    EICRA |= (1 << ISC00);
    // Enable external interrupt INT0
    EIMSK |= (1 << INT0);
}

void hcsr04_trigger(void) {
    // Generate 10us trigger pulse
    PORTB |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIG_PIN);
}

uint16_t hcsr04_get_distance_cm(void) {
    return distance_cm;
}

// Interrupt Service Routine for Echo pin state change
ISR(INT0_vect) {
    if (PIND & (1 << ECHO_PIN)) {
        // Rising edge - echo started
        pulse_start = TCNT1;
        edge_state = 1;
    } else {
        // Falling edge - echo ended
        pulse_end = TCNT1;
        edge_state = 0;
        
        uint16_t pulse_width = 0;
        if (pulse_end > pulse_start) {
            pulse_width = pulse_end - pulse_start;
        } else {
            // Handle timer overflow
            pulse_width = (0xFFFF - pulse_start) + pulse_end;
        }
        
        // Time in us = pulse_width * 0.5 (since prescaler 8 at 16MHz)
        // Distance in cm = (Time in us) / 58
        distance_cm = (pulse_width / 2) / 58;
    }
}
