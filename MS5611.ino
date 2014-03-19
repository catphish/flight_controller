void msGetPressure() {
  float t, p;
  barometer.readValues(&p, &t);
  pressure = p * 1000;
}

void msSetup() {
  float t, p;
  barometer.initialize();
  barometer.readValues(&p, &t);
  initial_pressure  = p * 1000;
}

