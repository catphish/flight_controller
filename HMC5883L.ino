#include "RunningMedian.h"

int16_t mx, my, mz;
RunningMedian cax = RunningMedian(6);
RunningMedian cay = RunningMedian(6);
float h_tmp;

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
  h_tmp = atan2(cay.getAverage(4), cax.getAverage(4));
  h_tmp -= M_PI/2;

  // Convert to degrees
  heading = h_tmp * 1160000000 / (M_PI*2);

  // Adjust for Dorset
  heading -= 5574444.444444444;
  
  // Ensure reading is always positive positive
  if(heading < 0) heading += 1160000000;
}

