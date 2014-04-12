// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Important definitions and main loop live here

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MS561101BA.h"
#include "HMC5883L.h"
#include "Wire.h"

// Settings
#define POSITION_FEEDBACK 280.0       // This is the pitch/roll feedback amount
#define POSITION_FEEDBACK_Z 50        // This is the yaw feedback amount
#define GYRO_FEEDBACK  0.10           // Rotational velovity correction
#define GYRO_FEEDBACK_Z  0.07         // Yaw velovity correction
#define CONTROL_SENSITIVITY 0.5       // Pitch/roll control sensitivity
#define CONTROL_SENSITIVITY_Z 1.0     // Yaw control sensitivity
#define INTEGRATION_AMOUNT 0.002      // Pitch/roll integration amount
#define ALTITUDE_HOLD_ADJUSTMENT 350  // Throttle to add based on pitch/roll

// RC Channels
long channel_1;
long channel_2;
long channel_3;
long channel_4;
long channel_5;
long channel_6;
long channel_7;

// Global Variables and Objects
double smoothed_control_x=0;          // Smoothed RC Input
double smoothed_control_y=0;          // Smoothed RC Input
double smoothed_control_t=0;          // Smoothed RC Input
double smoothed_control_z=0;          // Smoothed RC Input

int armed=0;                          // Motors enabled
double pos_x, pos_y;                  // IMU input
long int heading;
boolean upside_down;
double gyro_x, gyro_y;                // Gyro Input
double output_x, output_y, output_z;  // Stabilization Output
double altitude_hold_correction;      // Altitude hold output
double altitude_hold_control;         // Altitude hold amount selection
int n=0;

int32_t gyro_data_z;
long int integrated_gyro_data_z;
double pos_z_rad;

int accel_x;                          // Measured horizontal acceleration
int accel_y;                          // Measured horizontal acceleration
double velocity_estimate_x;           // Integrated acceleration (velocity x)
double velocity_estimate_y;           // Integrated acceleration (velocity y)

int accel_z;                          // Measured vertical acceleration
double velocity_estimate;             // Integrated vertical acceleration (velocity)
double altitude_estimate;             // Integrated vertical velocity (altitude)
double prev_aii;                      // Used to calculate velocity correction
double altitude_error;                // Used to calculate velocity correction
double pressure;                      // Current measured pressure
double initial_pressure;              // Initial pressure
double baro_alt;                      // The measured altitude

double integrated_x=0;                // Integration of roll
double integrated_y=0;                // Integration of pitch

double gps_lat;
double gps_long;
double initial_gps_lat;
double initial_gps_long;
double gps_offset_lat;
double gps_offset_long;
double gps_offset_x;
double gps_offset_y;

int gps_enabled;
int gps_online;

MPU6050 mpu;                          // Motion processor
HMC5883L mag;                         // Compass
bool dmpReady = false;                // set true if MPU initizlization was successful
MS561101BA barometer(B1110111);       // Barometer

void loop()
{
  // Safety
  if (!dmpReady) return;
  
  // Fetch data
  mpuGetXY();
  getLocation();
  n++;
  if(n == 4) {
    msGetPressure();
    getHeading();
    n = 0;
  }
  
  // Process RC data
  process_rc_data();
  
  // Calculate proportial responses with gyro correction
  double x, y, z;
  x = smoothed_control_x - pos_x * POSITION_FEEDBACK;
  y = smoothed_control_y - pos_y * POSITION_FEEDBACK;
  
  if(gps_enabled) {
    // Calculate relatve GPS location
    gps_offset_lat  = (gps_lat  - initial_gps_lat)  * 200000;
    gps_offset_long = (gps_long - initial_gps_long) * 200000;
    
    // Rotate to x/y offset
    gps_offset_x = gps_offset_lat * sin(pos_z_rad) + gps_offset_long * cos(pos_z_rad);
    gps_offset_y = gps_offset_lat * cos(pos_z_rad) + gps_offset_long * sin(pos_z_rad);
    
    x -= gps_offset_x;
    y -= gps_offset_y;
  }
  
  // Integrate pitch and roll to counter any permanent imbalance
  integrated_x += x * INTEGRATION_AMOUNT;
  integrated_y += y * INTEGRATION_AMOUNT;
  
  // Reset integrals and yaw if throttle is zero
  if(!armed) {
    integrated_x = 0;
    integrated_y = 0;
    smoothed_control_z = (integrated_gyro_data_z = heading) / 3222222.222222222;
  }
  
  // Calculate basic yaw correction
  z = smoothed_control_z - (integrated_gyro_data_z / 3222222.222222222);
  if (z >  180) z -= 360; // Try to find the most efficient path to correct yaw
  if (z < -180) z += 360; // Try to find the most efficient path to correct yaw
  
  // Apply the integrals
  output_x = x + integrated_x - gyro_x * GYRO_FEEDBACK;
  output_y = y + integrated_y - gyro_y * GYRO_FEEDBACK;
  
  // Don't correct yaw difference at extremes of pitch/roll, gimbal lock makes a mess of this
  z *= pow((1.570795 - abs(pos_x) - abs(pos_y)) / 1.570795, 2);
  
  // Adjusting yaw upside down is inaccurate and really quite pointless
  if(upside_down) z = 0;
  
  // Apply the yaw gyro feedback
  output_z = z * POSITION_FEEDBACK_Z + (gyro_data_z * GYRO_FEEDBACK_Z) / 512;
  
  // Limit extremes of yaw correction
  if(output_z < -200) output_z = -200;  if(output_z >  200) output_z =  200;
  
  // Calculate throttle correction based on pitch/roll and control input
  // 310 Seems to be a good value for this
  altitude_hold_correction = (abs(pos_x) + abs(pos_y)) * ALTITUDE_HOLD_ADJUSTMENT;
  
  // Calculate altitude and vertical velocity
  baro_alt = (initial_pressure - pressure);
  velocity_estimate += accel_z * 0.005;
  altitude_estimate += velocity_estimate * 0.005;
  
  // Correct with barometer
  prev_aii = altitude_estimate;
  altitude_error = baro_alt - altitude_estimate;
  altitude_estimate += altitude_error * 0.01;
  velocity_estimate += altitude_error * 0.03;
  
  // Apply altitude corrections
  altitude_hold_correction -= velocity_estimate * 0.5 + baro_alt * 0.5 - altitude_hold_control * 1;
  if(altitude_hold_correction < -300) altitude_hold_correction = -300;
  if(altitude_hold_correction >  300) altitude_hold_correction =  300;
  
  // Push data to motors
  set_velocities();
}

