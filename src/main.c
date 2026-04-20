#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "hcsr04.h"

// Define servo PWM pin (Timer2)
#define SERVO_PIN PB3

void servo_init() {
    // Set SERVO_PIN as output
    DDRB |= (1 << SERVO_PIN);
    
    // Fast PWM mode, non-inverting on OC2A (PB3)
    // Prescaler 1024 to get a 50Hz signal (20ms period) required by standard servos
    TCCR2A |= (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
}

void servo_set_angle(uint8_t angle) {
    // Map 0-180 degrees to roughly 1ms-2ms pulse width using 8-bit timer
    // For 16MHz clock and 1024 prescaler: 
    // 1 tick = 64us. 
    // 1ms = ~15 ticks. 2ms = ~31 ticks.
    uint8_t duty = 15 + ((angle * 16) / 180);
    OCR2A = duty;
}

int main(void) {
    uart_init(115200);
    hcsr04_init();
    servo_init();

    sei(); // Enable global interrupts

    uint8_t angle = 0;
    int8_t step = 5;

    while (1) {
        servo_set_angle(angle);
        
        // Wait for servo to reach position mechanically
        _delay_ms(50);
        
        // Trigger ultrasonic pulse
        hcsr04_trigger();
        
        // Wait for measurement to complete
        _delay_ms(20);
        
        uint16_t distance_cm = hcsr04_get_distance_cm();
        
        // Stream data over UART to the Host PC for 2D mapping
        printf("%d,%d\n", angle, distance_cm);

        // Sweep logic
        angle += step;
        if (angle >= 180 || angle <= 0) {
            step = -step; // Reverse direction
        }
    }
    return 0;
}
