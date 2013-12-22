// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define POSITION_FEEDBACK 300.0     // This is the pitch/roll feedback amount
#define GYRO_FEEDBACK  0.06          // Rotational velovity correction
#define YAW_FEEDBACK 0.3            // The yaw correction
#define X_CONTROL_SENSITIVITY 0.6   // Roll control sensitivity
#define Y_CONTROL_SENSITIVITY 0.6   // Pitch control sensitivity
#define Z_CONTROL_SENSITIVITY 2.0   // Yaw control sensitivity
#define ALTITUDE_HOLD_AMOUNT 150    // How much throttle should we add to compensate for angle
#define INTEGRATION_AMOUNT 0.001

// Global Variables and Objects
double smoothed_control_x=0, smoothed_control_y=0;       // Smoothed RC Input
double smoothed_control_t=0, smoothed_control_z=0;       // Smoothed RC Input
int armed=0;                                             // Motors enabled

double pos_x, pos_y;                     // IMU input
double gyro_x, gyro_y, gyro_z;           // Gyro Input
double output_x, output_y, output_z;     // Stabilization Output
double altitude_hold_correction;
double integrated_x=0;
double integrated_y=0;

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
  double x, y;
  x = smoothed_control_x - pos_x * POSITION_FEEDBACK  - gyro_x * GYRO_FEEDBACK;
  y = smoothed_control_y - pos_y * POSITION_FEEDBACK  - gyro_y * GYRO_FEEDBACK;
  output_x = x + integrated_x;
  output_y = y + integrated_y;
  integrated_x += x * INTEGRATION_AMOUNT;
  integrated_y += y * INTEGRATION_AMOUNT;
  if(smoothed_control_t < 100) integrated_x = 0;
  if(smoothed_control_t < 100) integrated_y = 0;
  
  output_z = smoothed_control_z - gyro_z * YAW_FEEDBACK;
  
  altitude_hold_correction = sqrt(sq(pos_x) + sq(pos_y)) * ALTITUDE_HOLD_AMOUNT;
  
  // Push data to motors
  set_velocities();
  
//  Serial.print(integrated_x);
//  Serial.print(",");
//  Serial.print(integrated_y);
//  Serial.print("\n");
}

