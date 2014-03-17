float base_pressure=0;

void msGetPressure() {
  float t, p;
  barometer.readValues(&p, &t);
  pressure = pressure * 0.9 + p * 10;
}
