void msGetPressure() {
  float t, p;
  barometer.readValues(&p, &t);
  if ((p * 1000) > (pressure * 0.9) and (p * 1000) < (pressure * 1.1)) {
    pressure = pressure * 0.9 + p * 100;
  }
}

void msSetup() {
  float t, p;
  barometer.initialize();
  barometer.readValues(&p, &t);
  pressure = initial_pressure = p * 1000;
}

