// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include <Servo.h>
#include "PID_v1.h"
#include "I2Cdev.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"

double control_x=0, control_y=0, control_r=0, control_z=0;
double roll_input, pitch_input;
double roll_output, pitch_output;

//PID pid_roll (&roll_input,  &roll_output,  &control_x, 0.025, 0.025, 0.025, REVERSE);
//PID pid_pitch(&pitch_input, &pitch_output, &control_y, 0.025, 0.025, 0.025, REVERSE);
PID pid_roll (&roll_input,  &roll_output,  &control_x, 0.03, 0.03, 0.03, REVERSE);
PID pid_pitch(&pitch_input, &pitch_output, &control_y, 0.03, 0.03, 0.03, REVERSE);

bool dmpReady = false; // set true if DMP init was successful

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;
BMP085 barometer;

#define X_CONTROL_SENSITIVITY 1
#define Y_CONTROL_SENSITIVITY 1
#define R_CONTROL_SENSITIVITY 0.2
#define GYRO_X_OFFSET  -0.0036
#define GYRO_Y_OFFSET -0.0008
#define GYRO_SENSITIVITY 1000

Servo front_left;
Servo front_right;
Servo rear_left;
Servo rear_right;

float pressure;
float initial_pressure;
int32_t lastMicros;

int n;

void loop()
{
  if (!dmpReady) return;
  mpuGetXY();

  //control_z = 0;
  //control_x = control_y = 0;

  pid_pitch.Compute(); pid_roll.Compute();

  //bmpGetPressure();
  set_velocities();
}


