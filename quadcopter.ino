// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define PID_P 200.0  // This is the yaw/roll feedback amount
#define PID_I 0.0    // This doesn't seem necessary and isn't currently used
#define PID_D 0.04   // This isn't really derived, it's a raw gyro, but the affect is the same
#define PIDZ_P 0.2   // The yaw feedback sensitivity
#define PIDZ_I 0.002 // Yaw has an I term to handle motor imbalance
#define X_CONTROL_SENSITIVITY 0.1 // X control sensitivity
#define Y_CONTROL_SENSITIVITY 0.1 // Y control sensitivity
#define Z_CONTROL_SENSITIVITY 0.6  // Z control sensitivity

// Global Variables and Objects
int control_x=0, control_y=0, control_t=0, control_z=0;  // RC Input
double smoothed_control_x=0, smoothed_control_y=0;       // Smoothed RC Input
double smoothed_control_t=0, smoothed_control_z=0;       // Smoothed RC Input
double pos_x, pos_y;                     // IMU input
double gyro_x, gyro_y, gyro_z;           // Gyro Input
double output_x, output_y, output_z;     // PID Output
double iterm_z=0;

MPU6050 mpu;            // Motion processor
BMP085 barometer;       // Barometer
bool dmpReady = false;  // set true if DMP init was successful
float pressure;         // Current pressure
float initial_pressure; // Initial pressure
float battery_voltage;  // Battery voltage
int n=0;
long t;

void loop()
{
  // Safety
  if (!dmpReady) return;
  
  // Fetch data
  mpuGetXY();
  bmpGetPressure();
  battery_voltage = analogRead(8) * 5.0 / 1023;
  // Process data
  process_rc_data();
  
  // PID
  output_x = smoothed_control_x - pos_x * PID_P  - gyro_x * PID_D;
  output_y = smoothed_control_y - pos_y * PID_P  - gyro_y * PID_D;
  //iterm_z += gyro_z * PIDZ_I;
  output_z = smoothed_control_z - gyro_z * PIDZ_P;// - iterm_z;
  
  // Push data to motors
  n++;
  if (n > 9) {
    n = 0;
    set_velocities();
  }
}
