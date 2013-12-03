// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Custom routines relating to the BPM085 to get the current pressure when available

float temperature;
long lastMicros=0;
bool tp;

void bmpGetPressure() {
  if(lastMicros == 0) {
    barometer.setControl(BMP085_MODE_TEMPERATURE);
    lastMicros = micros();
    tp = false;
  } else if (micros() - lastMicros > barometer.getMeasureDelayMicroseconds()) {
    if(tp == false) {
      temperature = barometer.getTemperatureC();
      barometer.setControl(BMP085_MODE_PRESSURE_3);
      tp = true;
    } else {
      if (pressure == 0.0) {
        pressure = barometer.getPressure();
      } else {
        pressure = pressure * 0.9 + barometer.getPressure() * 0.1;
      }
      if (initial_pressure == 0.0) {
        initial_pressure = pressure;
      }
      barometer.setControl(BMP085_MODE_TEMPERATURE);
      tp = false;
    }
    lastMicros = micros();
  }
}
