MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)

PORT = COM3
BAUD = 115200
PROGRAMMER = arduino

SRCS = src/main.c src/uart.c src/hcsr04.c
OBJS = $(SRCS:.c=.o)
TARGET = radar

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	avrdude -F -V -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<

clean:
	rm -f src/*.o *.elf *.hex
