// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "settings.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "serial.h"
#include "timers.h"
#include "radio.h"
#include "i2c.h"
#include "mpu6050.h"
#include "motors.h"

int main(void)
{
  // General variables
  float input_y,  input_p,  input_r;
  int   output_y, output_p, output_r;
  int16_t gyro_x, gyro_y, gyro_z;
  int throttle, armed;
  
  // Setup process
  cli();          // Switch interrupts off during configuration
  DDRA = 0x00;    // PORT A is INPUT
  DDRD = 0xF0;    // PORT D is OUTPUT
  PORTA=0;        // Don't know if this is needed on inputs
  PORTD=0;        // Clear any motor outputs
  timers_init();  // Set up timers
  pci_init();     // Set up interrupts
  uart_init();    // Set up serial for debugging
  i2c_init();     // Initialize the I2C bus
  sei();          // Turn interrupts back on
  mpu6050_init(); // Set up MPU6050
  
  // Main loop
  for(;;) {
    // Fetch data
    mpu6050_get_ypr(&input_y, &input_p, &input_r);
    mpu6050_get_gyro(&gyro_x, &gyro_y, &gyro_z);
    
    // Calculations
    armed = channel_5 > 1500;
    throttle = IDLE_SPEED + channel_3 * THROTTLE_SENSITIVITY;
    output_y = 0;
    output_p = 0;
    output_r = 0;
    
    // Push data to motors
    update_motors(armed, output_r, output_p, output_y, throttle);
  }
}

