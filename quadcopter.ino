// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "BMP085.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// Settings
#define PID_P 200.0
#define PID_I 0.0
#define PID_D 0.04
#define PIDZ_P 3.0
#define PIDZ_I 0.0
#define PIDZ_D 0.2
#define X_CONTROL_SENSITIVITY 0.1
#define Y_CONTROL_SENSITIVITY 0.1
#define Z_CONTROL_SENSITIVITY 0.2

// Global Variables and Objects
int control_x=0, control_y=0, control_t=0, control_z=0;  // RC Input
double smoothed_control_x=0, smoothed_control_y=0;       // Smoothed RC Input
double smoothed_control_t=0, smoothed_control_z=0;       // Smoothed RC Input
double pos_x, pos_y;                     // IMU input
double gyro_x, gyro_y, gyro_z;           // Gyro Input
double output_x, output_y, output_z;     // PID Output

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
  output_z = smoothed_control_z         * PIDZ_P - gyro_z * PIDZ_D;
  
  // Push data to motors
  n++;
  if (n > 9) {
    n = 0;
    set_velocities();
  }
}

