// Settings
#define F_CPU 16000000L
#define BAUD 57600
#define POSITION_FEEDBACK 0.3            // This is the pitch/roll feedback amount
#define GYRO_FEEDBACK  0.1               // Rotational velovity correction
#define GYRO_FEEDBACK_YAW  0.1           // Yaw velovity correction
#define CONTROL_SENSITIVITY 0.3          // Pitch/roll control sensitivity
#define CONTROL_SENSITIVITY_YAW 0.3      // Yaw control sensitivity
#define INTEGRATION_AMOUNT 0.002         // Pitch/roll integration amount
#define IDLE_SPEED 200                   // Minimum (idle) throttle when armed
#define THROTTLE_SENSITIVITY 0.4         // Sensitivity of throttle
#define ALTITUDE_CORRECTION 1.0          // Amount to correct throttle to compensate pitch/roll

