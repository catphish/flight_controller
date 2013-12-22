// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define POSITION_FEEDBACK 150.0     // This is the pitch/roll feedback amount
#define GYRO_FEEDBACK  0.03         // Rotational velovity correction
#define YAW_FEEDBACK 0.1            // The yaw correction
#define X_CONTROL_SENSITIVITY 0.25  // Roll control sensitivity
#define Y_CONTROL_SENSITIVITY 0.25  // Pitch control sensitivity
#define Z_CONTROL_SENSITIVITY 1.0   // Yaw control sensitivity
#define ALTITUDE_HOLD_AMOUNT 100    // How much throttle should we add to compensate for angle

// Global Variables and Objects
double smoothed_control_x=0, smoothed_control_y=0;       // Smoothed RC Input
double smoothed_control_t=0, smoothed_control_z=0;       // Smoothed RC Input
int armed=0;                                             // Motors enabled

double pos_x, pos_y;                     // IMU input
double gyro_x, gyro_y, gyro_z;           // Gyro Input
double output_x, output_y, output_z;     // Stabilization Output
double altitude_hold_correction;

MPU6050 mpu;             // Motion processor
bool dmpReady = false;   // set true if DMP init was successful

void loop()
{
  // Safety
  if (!dmpReady) return;
  
  // Fetch data
  mpuGetXY();
  
  // Process data
  process_rc_data();
  output_x = smoothed_control_x - pos_x * POSITION_FEEDBACK  - gyro_x * GYRO_FEEDBACK;
  output_y = smoothed_control_y - pos_y * POSITION_FEEDBACK  - gyro_y * GYRO_FEEDBACK;
  output_z = smoothed_control_z - gyro_z * YAW_FEEDBACK;
  altitude_hold_correction = sqrt(sq(pos_x) + sq(pos_y)) * ALTITUDE_HOLD_AMOUNT;
  
  // Push data to motors
  set_velocities();
  
}

