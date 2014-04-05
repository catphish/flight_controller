#include "RunningMedian.h"

int16_t mx, my, mz;
RunningMedian cax = RunningMedian(6);
RunningMedian cay = RunningMedian(6);

void getHeading() {
  // Read raw heading measurements from device
  mag.getHeading(&mx, &my, &mz);

  float xh = mx * cos(pos_x) + mz * sin(pos_x);
  float yh = mx * sin(-pos_y) * sin(pos_x) + my * cos(-pos_y) - mz * sin(-pos_y) * cos(pos_x);
  
  // Adjust for my device
  xh -= 66;
  yh += 116;
  
  float norm = sqrt(xh * xh + yh * yh);
  xh = xh / norm;
  yh = yh / norm;
  
  // Filter
  cax.add(xh);
  cay.add(yh);

  // To calculate heading in degrees. 0 degree indicates North
  heading = atan2(cay.getAverage(4), cax.getAverage(4));
  heading -= M_PI/2;

  // Convert to degrees
  heading *= 180/M_PI;

  // Adjust for Dorset
  heading -= 1.73;
  
  // Ensure reading is always positive positive
  if(heading < 0) heading += 360;
}

