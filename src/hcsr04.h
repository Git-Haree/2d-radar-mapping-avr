#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>

// Trigger on PB0, Echo on PD2 (INT0)
#define TRIG_PIN PB0
#define ECHO_PIN PD2

void hcsr04_init(void);
void hcsr04_trigger(void);
uint16_t hcsr04_get_distance_cm(void);

#endif // HCSR04_H
