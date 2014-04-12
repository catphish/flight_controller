#include "RunningMedian.h"

int16_t mx, my, mz;

void getHeading() {
  // Read raw heading measurements from device
  mag.getHeading(&mx, &my, &mz);
  
  float xh = mx * cos(-pos_x) + mz * sin(-pos_x);
  float yh = mx * sin(pos_y) * sin(-pos_x) + my * cos(pos_y) - mz * sin(pos_y) * cos(-pos_x);
  
  // Adjust for my device
  xh -= 66;
  yh += 116;
  
  // To calculate heading in degrees. 0 degree indicates North
  pos_z_rad = atan2(xh, yh);
  
  // Rotate to magnetic north
  pos_z_rad += M_PI;
  
  // Rotate to true north (Dorset)
  pos_z_rad -= 0.030194196;
  
  // Convert to bigDegrees (This unit is the result of my gyro scale)
  heading = pos_z_rad * 1160000000 / (M_PI*2);
  
  // Ensure reading is always positive positive
  if(heading < 0)          heading += 1160000000;
  if(heading > 1160000000) heading -= 1160000000;
  
}

