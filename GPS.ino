#include "nmea.h"
NMEA gps(GPRMC);

void getLocation() {
  while(Serial3.available()) {
    char c = Serial3.read();
    // Pass each byte to the NMEA library and let it tell us when a sentence has been decoded
    if (gps.decode(c)) {
      // check if GPS positioning was active
      if (gps.gprmc_status() == 'A') {
        gps_lat  = gps.gprmc_latitude();
        gps_long = gps.gprmc_longitude();
        gps_online = 1;
      } else {
        gps_online = 0;
      }
    }
  }
}
