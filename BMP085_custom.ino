// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Custom routines relating to the BPM085 to get the current pressure when available

float temperature;
long lastMicros;

void bmpGetPressure() {
  barometer.setControl(BMP085_MODE_TEMPERATURE);
  lastMicros = micros();
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());
  temperature = barometer.getTemperatureC();

  barometer.setControl(BMP085_MODE_PRESSURE_3);
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());
  pressure = barometer.getPressure();
}


