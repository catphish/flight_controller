// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include <Servo.h>
#include "I2Cdev.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"

bool dmpReady = false; // set true if DMP init was successful

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;
BMP085 barometer;

#define X_CONTROL_SENSITIVITY 0.1
#define Y_CONTROL_SENSITIVITY 0.1
#define R_CONTROL_SENSITIVITY 0.2
#define GYRO_SENSITIVITY 20
#define GYRO_MULTIPLIER 5
#define GYRO_CURVE 0.8

Servo front_left;
Servo front_right;
Servo rear_left;
Servo rear_right;

float pressure;
float initial_pressure;
int32_t lastMicros;

long x=0L,y=0L;
long control_x=0L, control_y=0L, control_r=0L, control_z=0L;

int n;

void loop()
{
  if (!dmpReady) return;
  mpuGetXY();
  //bmpGetPressure();
  set_velocities();
}


