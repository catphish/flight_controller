// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "PID_v1.h"
#include "I2Cdev.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define PID_P 0.7
#define PID_I 0.7
#define PID_D 0.00
#define X_CONTROL_SENSITIVITY 0.03
#define Y_CONTROL_SENSITIVITY 0.03
#define R_CONTROL_SENSITIVITY 0.03
#define GYRO_X_OFFSET  0.0129
#define GYRO_Y_OFFSET -0.0183
#define ACCEL_SENSITIVITY 100
#define GYRO_SENSITIVITY 0.025

// Global Variables and Objects
int control_x=0, control_y=0, control_z=0, control_r=0;  // RC Input
double smoothed_control_x=0, smoothed_control_y=0;       // Smoothed RC Input
double smoothed_control_z=0, smoothed_control_r=0;       // Smoothed RC Input
double roll_input, pitch_input;   // Accelerometer input
double gyro_x, gyro_y;            // Gyro Input
double roll_mid, pitch_mid;       // PID 1 Output
double roll_output, pitch_output; // PID 2 Output
PID pid_roll_a  (&roll_input,  &roll_mid,     &smoothed_control_x,    PID_P, PID_I, PID_D, DIRECT); // PID 1
PID pid_pitch_a (&pitch_input, &pitch_mid,    &smoothed_control_y,    PID_P, PID_I, PID_D, DIRECT); // PID 1
PID pid_roll_b  (&gyro_x,      &roll_output,  &roll_mid,              1,     0,      0,    DIRECT); // PID 2
PID pid_pitch_b (&gyro_y,      &pitch_output, &pitch_mid,             1,     0,      0,    DIRECT); // PID 2
MPU6050 mpu;            // Motion processor
BMP085 barometer;       // Barometer
bool dmpReady = false;  // set true if DMP init was successful
float pressure;         // Current pressure
float initial_pressure; // Initial pressure

void loop()
{
  // Safety
  if (!dmpReady) return;
  
  // Fetch data
  mpuGetXY();
  bmpGetPressure();
  
  // Process data
  process_rc_data();
  pid_pitch_a.Compute(); pid_roll_a.Compute();
  pid_pitch_b.Compute(); pid_roll_b.Compute();
  
  // Push data to motors
  set_velocities();

  // Wait a while between iterations
  delay(10);
}



