boolean base_pressure_set=false;
float base_pressure=0;

void msGetPressure() {
  float t, p;
  barometer.readValues(&p, &t);
  if(!base_pressure_set) {
    base_pressure = p;
    base_pressure_set = true;
  }
  pressure = pressure * 0.9 + (p - base_pressure) * 10;
}
