// Experimental Quadcopter flight controller
// by Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
// Based on Code by Jeff Rowberg <jeff@rowberg.net>
//
// Definitions and custom routines relating to the MPU6050 and DMP

#include <math.h>

// MPU control/status vars
uint8_t mpuIntStatus; // holds actual interrupt status byte from MPU
uint8_t devStatus; // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize; // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount; // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q; // [w, x, y, z] quaternion container
VectorInt16 aa; // [x, y, z] accel sensor measurements
VectorInt16 aaReal; // [x, y, z] gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z] world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z] gravity vector
float euler[3]; // [psi, theta, phi] Euler angle container
float ypr[3]; // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector

void ntohl(uint32_t* value) {
  uint32_t tmp_a = (*value & 0xff000000) >> 24;
  uint32_t tmp_b = (*value & 0x00ff0000) >> 8;
  uint32_t tmp_c = (*value & 0x0000ff00) << 8 ;
  uint32_t tmp_d = (*value & 0x000000ff) << 24;
  *value = tmp_d | tmp_c |tmp_b | tmp_a;
}

void mpuGetXY() {
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    //Serial.println(F("FIFO overflow!"));
  }
  else if (mpuIntStatus & 0x02) {
    while (fifoCount >= packetSize) {
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;
      // Get pitch and roll from DMP
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetAccel(&aa, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
      mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
      
      accel_x = aaReal.x;
      accel_y = aaReal.y;
      accel_z = aaWorld.z;
      
      pos_x =  ypr[2];
      pos_y = -ypr[1];
      // Get gyro data from MPU
      int ax, ay, az, gx, gy, gz;
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      
      gyro_x = (gx);
      gyro_y = (gy);
      
      mpu.dmpGetEuler(euler, &q);
      upside_down = (abs(euler[2]) > 1.570795);
      
      // Fetch high resolution Z gyro data
      memcpy(&gyro_data_z, fifoBuffer+24, 4);
      ntohl((uint32_t*)&gyro_data_z);
      integrated_gyro_data_z -= gyro_data_z;
      
      // Integrate rotation
      if (integrated_gyro_data_z > 1160000000) integrated_gyro_data_z -= 1160000000;
      if (integrated_gyro_data_z < 0  ) integrated_gyro_data_z += 1160000000;
      
      // Correct with compass data
      float error_z = heading - integrated_gyro_data_z;
      if (error_z >  580000000) error_z -= 1160000000; // Try to find the most efficient path to correct yaw
      if (error_z < -580000000) error_z += 1160000000; // Try to find the most efficient path to correct yaw
      if (error_z >  1000) error_z =  1000;
      if (error_z < -1000) error_z = -1000;
      integrated_gyro_data_z += error_z;
      
      // Calculate in radians
      pos_z_rad = integrated_gyro_data_z / 184619733.986598604;
    }
  }
}

