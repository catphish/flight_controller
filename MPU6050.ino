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
      accel_z = aaWorld.z;
      
      pos_x =  ypr[2];
      pos_y = -ypr[1];
      pos_z =  ypr[0] * 180 / 3.14159;
      if(pos_z < 0.0) pos_z += 360.0;
      // Get gyro data from MPU
      int ax, ay, az, gx, gy, gz;
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      gyro_x = (gx);
      gyro_y = (gy);
      gyro_z = (-gz);
      
      mpu.dmpGetEuler(euler, &q);
      upside_down = (abs(euler[2]) > 1.570795);
    }
  }
}

