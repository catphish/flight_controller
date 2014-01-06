// Settings
#define F_CPU 16000000L
#define BAUD 57600
#define POSITION_FEEDBACK 300.0       // This is the pitch/roll feedback amount
#define POSITION_FEEDBACK_Z 20        // This is the yaw feedback amount
#define GYRO_FEEDBACK  0.06           // Rotational velovity correction
#define GYRO_FEEDBACK_YAW  0.1        // Yaw velovity correction
#define CONTROL_SENSITIVITY 0.6       // Pitch/roll control sensitivity
#define CONTROL_SENSITIVITY_YAW 1.0   // Yaw control sensitivity
#define INTEGRATION_AMOUNT 0.002      // Pitch/roll integration amount
#define IDLE_SPEED 200                // Minimum (idle) throttle when armed
#define THROTTLE_SENSITIVITY 0.5      // Sensitivity of throttle
