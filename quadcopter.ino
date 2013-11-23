// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include <Servo.h>
#include "PID_v1.h"
#include "I2Cdev.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"

// RC Input
int control_x=0, control_y=0, control_z=0, control_r=0;
// Smoothed RC Input
double smoothed_control_x=0, smoothed_control_y=0, smoothed_control_z=0, smoothed_control_r=0;
// Dummy zero target
double target = 0;

// Accelerometer input
double roll_input, pitch_input;
// Gyro Input
double gyro_x, gyro_y;
// PID 1 Output
double roll_mid, pitch_mid;
// PID 2 Output
double roll_output, pitch_output;

#define PIDP 1
#define PIDI 1
#define PIDD 0.00

PID pid_roll_a  (&roll_input,  &roll_mid,     &target,    PIDP, PIDI, PIDD, DIRECT);
PID pid_pitch_a (&pitch_input, &pitch_mid,    &target,    PIDP, PIDI, PIDD, DIRECT);
PID pid_roll_b  (&gyro_x,      &roll_output,  &roll_mid,  1, 0, 0, DIRECT);
PID pid_pitch_b (&gyro_y,      &pitch_output, &pitch_mid, 1, 0, 0, DIRECT);

bool dmpReady = false; // set true if DMP init was successful

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;
BMP085 barometer;

#define X_CONTROL_SENSITIVITY 0
#define Y_CONTROL_SENSITIVITY 0
#define R_CONTROL_SENSITIVITY 0
#define GYRO_X_OFFSET  0.0129
#define GYRO_Y_OFFSET -0.0183
#define ACCEL_SENSITIVITY 100
#define GYRO_SENSITIVITY 0.03

//Servo front_left;
//Servo front_right;
//Servo rear_left;
//Servo rear_right;

float pressure;
float initial_pressure;
int32_t lastMicros;

int n;
long timer;

void loop()
{
  if (!dmpReady) return;
  mpuGetXY();
  smoothed_control_x = smoothed_control_x * 0.95 + control_x * 0.05;
  smoothed_control_y = smoothed_control_y * 0.95 + control_y * 0.05;
  smoothed_control_z = smoothed_control_z * 0.95 + (control_z - 1144) * 0.05;
  smoothed_control_r = smoothed_control_r * 0.95 + control_r * 0.05;
  //control_z = 0;
  //control_x = control_y = 0;

  pid_pitch_a.Compute(); pid_roll_a.Compute();
  pid_pitch_b.Compute(); pid_roll_b.Compute();

  //bmpGetPressure();
  set_velocities();
  delay(10);
  //Serial.println(control_x);
  

}


