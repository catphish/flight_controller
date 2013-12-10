// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Custom routines relating to the BPM085 to get the current pressure when available

long lastMicros=0;
bool tp;
double ground_pressure=0; // Initial pressure
double p=0;

void bmpGetPressure() {
  if(lastMicros == 0) {
    barometer.setControl(BMP085_MODE_TEMPERATURE);
    lastMicros = micros();
    tp = false;
  } else if (micros() - lastMicros > barometer.getMeasureDelayMicroseconds()) {
    if(tp == false) {
      barometer.getTemperatureC();
      barometer.setControl(BMP085_MODE_PRESSURE_3);
      tp = true;
    } else {
      p = barometer.getPressure();
      if (ground_pressure == 0.0) {
        ground_pressure = p;
      }
      pressure = pressure * 0.95 + (ground_pressure - p) * 0.05;
      
      barometer.setControl(BMP085_MODE_TEMPERATURE);
      tp = false;
    }
    lastMicros = micros();
  }
}

