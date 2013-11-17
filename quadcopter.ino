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

#define X_CONTROL_SENSITIVITY 0.25
#define Y_CONTROL_SENSITIVITY 0.25
#define X_GYRO_SENSITIVITY 1000
#define Y_GYRO_SENSITIVITY 1000

Servo front_left;
Servo front_right;
Servo rear_left;
Servo rear_right;

float pressure;
float initial_pressure;
int32_t lastMicros;

long x=0L,y=0L;
long control_x, control_y, control_r, control_z;
long control_x_center=-1L, control_y_center=-1L, control_r_center=-1L, control_z_center=-1L;

int n;

void loop()
{
  //if (!dmpReady) return;
  //mpuGetXY();
  bmpGetPressure();
  set_velocities();
}


