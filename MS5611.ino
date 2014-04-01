#include "RunningMedian.h"
RunningMedian samples = RunningMedian(10);

void msGetPressure() {
  float t, p;
  barometer.readValues(&p, &t);
  samples.add(p*1000);
  pressure = samples.getAverage(6);
}

void msSetup() {
  float t, p;
  barometer.initialize();
  barometer.readValues(&p, &t);
  pressure = initial_pressure = p * 1000;
}

