// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include <Servo.h>
#include "PID_v1.h"
#include "I2Cdev.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"

int control_x=0, control_y=0, control_z=0, control_r=0;
double smoothed_control_x=0, smoothed_control_y=0, smoothed_control_z=0, smoothed_control_r=0;
double target = 0;

double roll_input, pitch_input;
double roll_output, pitch_output;
double roll_output_p, pitch_output_p;
double roll_output_i, pitch_output_i;
double roll_output_d, pitch_output_d;

#define PMIN -20
#define PMAX 20
#define IMIN -20
#define IMAX 20
#define DMIN -20
#define DMAX 20

//#define PIDP 0.08
//#define PIDI 0.01
//#define PIDD 0.015
#define PIDP 0.01
#define PIDI 0.005
#define PIDD 0.005

PID p_roll (&roll_input,  &roll_output_p,  &target, PIDP, 0, 0, REVERSE);
PID p_pitch(&pitch_input, &pitch_output_p, &target, PIDP, 0, 0, REVERSE);
PID i_roll (&roll_input,  &roll_output_i,  &target, 0, PIDI, 0, REVERSE);
PID i_pitch(&pitch_input, &pitch_output_i, &target, 0, PIDI, 0, REVERSE);
PID d_roll (&roll_input,  &roll_output_d,  &target, 0, 0, PIDD, REVERSE);
PID d_pitch(&pitch_input, &pitch_output_d, &target, 0, 0, PIDD, REVERSE);

bool dmpReady = false; // set true if DMP init was successful

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;
BMP085 barometer;

#define X_CONTROL_SENSITIVITY 1
#define Y_CONTROL_SENSITIVITY 1
#define R_CONTROL_SENSITIVITY 1
#define GYRO_X_OFFSET  0.0129
#define GYRO_Y_OFFSET -0.0183
#define GYRO_SENSITIVITY 1000

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

  p_pitch.Compute(); p_roll.Compute();
  i_pitch.Compute(); i_roll.Compute();
  d_pitch.Compute(); d_roll.Compute();

  pitch_output = pitch_output_p + pitch_output_i + pitch_output_d;
  roll_output =  roll_output_p  + roll_output_i  + roll_output_d;

  //bmpGetPressure();
  set_velocities();
  delay(10);
  //Serial.println(control_x);
  

}


