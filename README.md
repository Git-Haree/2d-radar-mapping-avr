# Real-Time 2D Radar Mapping System (AVR)

This project implements a low-cost, 2D proximity perception system using an ultrasonic Time-of-Flight (ToF) sensor (HC-SR04), a standard servo motor for scanning, and bare-metal AVR C on an ATmega328P. It serves as a foundational analog to ADAS (Advanced Driver Assistance Systems) radar pipelines.

## Architecture

The system uses hardware timers and Interrupt Service Routines (ISRs) to precisely calculate the microsecond echo return time of acoustic waves, translating them into distances. Data is continuously streamed over UART to a host perception script.

```mermaid
flowchart TD
    subgraph Microcontroller [ATmega328P Bare-Metal C]
        Timer1[16-bit Timer1\nToF Counter & ISR]
        Timer2[8-bit Timer2\nPWM Servo Sweep]
        UART[Hardware UART\n115200 Baud]
        
        Timer1 -->|Microsecond Capture| Dist[Distance Calc]
        Timer2 -->|Position Angle| Angle[Angle Calc]
        
        Dist --> UART
        Angle --> UART
    end

    subgraph Peripherals
        US[HC-SR04 Ultrasonic\nTrigger / Echo]
        Servo[Micro Servo\n180-degree Scan]
        
        Timer1 -.->|Trigger| US
        US -.->|Echo INT0| Timer1
        Timer2 -.->|PWM Signal| Servo
    end

    subgraph Host Software (Python)
        PySerial[pyserial\nData Intake]
        Matplotlib[matplotlib\nPolar Plotting]
        
        UART -.->|distance, angle| PySerial
        PySerial --> Matplotlib
    end
```

## Hardware Setup
1. **HC-SR04**: VCC to 5V, GND to GND. `Trig` to `PB0`, `Echo` to `PD2` (External Interrupt 0).
2. **Servo**: VCC to 5V, GND to GND. `Signal` to `PB3` (Timer2 PWM output).
3. **UART**: Standard USB-TTL adapter connected to `TX/RX` at 115200 baud.

## Software Architecture
The firmware is entirely interrupt-driven, meaning the main loop is free to handle data packaging.
- **`hcsr04.c`**: Manages the 10us trigger pulse on `PB0` and uses the `INT0` external interrupt to start/stop Timer1, accurately measuring the echo pulse width.
- **`uart.c`**: High-baud transmission to prevent queue bottlenecks during rapid sweeping.
- **`tools/radar_plot.py`**: A python script that reads the `(angle, distance)` tuples and generates a real-time 2D polar map.

## Building and Flashing
Standard `avr-gcc` toolchain is required.

```bash
make
make flash
```
