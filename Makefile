all:
	avr-gcc -Wall -mmcu=atmega2560 -Os -o quadcopter.elf quadcopter.c radio.c motors.c serial.c timers.c i2c.c mpu6050.c -lprintf_flt -lm
	avr-objcopy -j .text -j .data -O ihex quadcopter.elf quadcopter.hex

clean:
	rm quadcopter.elf quadcopter.hex

install:
	avrdude -p m2560 -c usbasp -e -U flash:w:quadcopter.hex
