// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define POSITION_FEEDBACK 300.0       // This is the pitch/roll feedback amount
#define POSITION_FEEDBACK_Z 20        // This is the yaw feedback amount
#define GYRO_FEEDBACK  0.06           // Rotational velovity correction
#define GYRO_FEEDBACK_Z  0.1          // Yaw velovity correction
#define CONTROL_SENSITIVITY 0.6       // Pitch/roll control sensitivity
#define CONTROL_SENSITIVITY_Z 1.0     // Yaw control sensitivity
#define INTEGRATION_AMOUNT 0.002      // Pitch/roll integration amount

// Global Variables and Objects
double smoothed_control_x=0;          // Smoothed RC Input
double smoothed_control_y=0;          // Smoothed RC Input
double smoothed_control_t=0;          // Smoothed RC Input
double smoothed_control_z=0;          // Smoothed RC Input

int armed=0;                          // Motors enabled
double pos_x, pos_y, pos_z;           // IMU input
boolean upside_down;
double gyro_x, gyro_y, gyro_z;        // Gyro Input
double output_x, output_y, output_z;  // Stabilization Output
double altitude_hold_correction;      // Altitude hold output
double altitude_hold_control;         // Altitude hold amount selection

double integrated_x=0;                // Integration of roll
double integrated_y=0;                // Integration of pitch

MPU6050 mpu;                          // Motion processor
bool dmpReady = false;                // set true if MPU initizlization was successful

void loop()
{
  // Safety
  if (!dmpReady) return;
  
  // Fetch data
  mpuGetXY();
  
  // Process RC data
  process_rc_data();
  
  // Calculate proportial responses with gyro correction
  double x, y, z;
  x = smoothed_control_x - pos_x * POSITION_FEEDBACK - gyro_x * GYRO_FEEDBACK;
  y = smoothed_control_y - pos_y * POSITION_FEEDBACK - gyro_y * GYRO_FEEDBACK;
  
  // Integrate pitch and roll to counter any permanent imbalance
  integrated_x += x * INTEGRATION_AMOUNT;
  integrated_y += y * INTEGRATION_AMOUNT;
  
  // Reset integrals and yaw if throttle is zero
  if(smoothed_control_t < 50) {
    integrated_x = 0;
    integrated_y = 0;
    smoothed_control_z = pos_z;
  }
  
  // Calculate basic yaw correction
  z = smoothed_control_z - pos_z;
  if (z >  180) z -= 360; // Try to find the most efficient path to correct yaw
  if (z < -180) z += 360; // Try to find the most efficient path to correct yaw
  
  // Apply the integrals
  output_x = x + integrated_x;
  output_y = y + integrated_y;
  
  // Don't correct yaw difference at extremes of pitch/roll, gimbal lock makes a mess of this
  z *= (1.570795 - abs(pos_x) - abs(pos_y)) / 1.570795;
  
  // Adjusting yaw upside down is inaccurate and really quite pointless
  if(upside_down) z = 0;
  
  // Apply the yaw gyro feedback
  output_z = z * POSITION_FEEDBACK_Z - gyro_z * GYRO_FEEDBACK_Z;
  
  // Limit extremes of yaw correction
  if(output_z < -300) output_z = -300;  if(output_z >  300) output_z =  300;

  // Calculate throttle correction based on pitch/roll and control input
  // 180 Seems to be a good value for this, but it's adjustable for now
  altitude_hold_correction = (abs(pos_x) + abs(pos_y)) * altitude_hold_control;
  
  // Push data to motors
  set_velocities();
}

