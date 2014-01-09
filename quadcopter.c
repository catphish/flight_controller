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
  int16_t gyro_y, gyro_p, gyro_r;
  int throttle, armed;
  
  // Setup process
  cli();          // Switch interrupts off during configuration
  DDRA = 0xFF;    // PORT A is OUTPUT
  PORTA=0x00;     // Clear any motor outputs
  //DDRD = 0x00;
  //PORTD=0;
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
    mpu6050_get_gyro(&gyro_y, &gyro_p, &gyro_r);
    
    // Calculations
    armed = channel_5 > 1500;
    throttle = IDLE_SPEED + (channel_3-1150) * THROTTLE_SENSITIVITY;
    output_y = (channel_4-1500) * CONTROL_SENSITIVITY_YAW;
    output_y -= gyro_y * GYRO_FEEDBACK_YAW;
    
    output_p = (channel_2-1500) * CONTROL_SENSITIVITY;
    output_p -= gyro_p * GYRO_FEEDBACK;
    output_p -= input_p * POSITION_FEEDBACK;

    output_r = (channel_1-1500) * CONTROL_SENSITIVITY;
    output_r -= gyro_r * GYRO_FEEDBACK;
    output_r -= input_r * POSITION_FEEDBACK;
    
    // Push data to motors
    update_motors(armed, output_r, output_p, output_y, throttle);
    
  }
}

