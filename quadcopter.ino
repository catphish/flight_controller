// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "HMC5883L.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define PID_P 150.0  // This is the yaw/roll feedback amount
#define PID_D 0.03   // This isn't really derived, it's a raw gyro, but the affect is the same
#define PIDZ_P 0.1   // The yaw feedback sensitivity
#define X_CONTROL_SENSITIVITY 0.25  // X control sensitivity
#define Y_CONTROL_SENSITIVITY 0.25  // Y control sensitivity
#define Z_CONTROL_SENSITIVITY 1.0  // Z control sensitivity

// Global Variables and Objects
double smoothed_control_x=0, smoothed_control_y=0;       // Smoothed RC Input
double smoothed_control_t=0, smoothed_control_z=0;       // Smoothed RC Input
int armed=0;

double pos_x, pos_y;                     // IMU input
double gyro_x, gyro_y, gyro_z;           // Gyro Input
double output_x, output_y, output_z;     // PID Output

MPU6050 mpu;             // Motion processor
BMP085 barometer;        // Barometer
HMC5883L mag;            // Compass
bool dmpReady = false;   // set true if DMP init was successful
double pressure;         // Current pressure
double initial_pressure; // Initial pressure
double distance;         // Current distance from ground if ultrasound installed
float battery_voltage;   // Battery voltage
float heading;           // Rotational direction
double altitude_hold_control;
double altitude_hold_correction;

void loop()
{
  // Safety
  if (!dmpReady) return;
  
  // Fetch data
  mpuGetXY();
  bmpGetPressure();
  //hmlGetHeading();   // Not used
  //sr04GetDistance(); // Not used
  battery_voltage = analogRead(8) * 5.0 / 1023;
  
  // Process data
  process_rc_data();
  output_x = smoothed_control_x - pos_x * PID_P  - gyro_x * PID_D;
  output_y = smoothed_control_y - pos_y * PID_P  - gyro_y * PID_D;
  output_z = smoothed_control_z - gyro_z * PIDZ_P;
  altitude_hold_correction = (pressure - initial_pressure) * altitude_hold_control * 0.02 + sqrt(sq(pos_x) + sq(pos_y)) * 100;
  // Push data to motors
  set_velocities();
  
  
}

