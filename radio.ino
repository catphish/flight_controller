// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Radio control interrupts and processing

unsigned char previous_pink=0;
unsigned char x=0;
unsigned long channel_1_prev;
unsigned long channel_2_prev;
unsigned long channel_3_prev;
unsigned long channel_4_prev;
unsigned long channel_5_prev;
unsigned long channel_6_prev;
unsigned long channel_7_prev;
int previous_gps_enabled;

double previous_altitude_hold_control=1000;

SIGNAL(PCINT2_vect) {
  x = PINK^previous_pink;
  if(x & 0x01) {
    if(previous_pink & 0x01) {
      channel_1 = micros() - channel_1_prev;
    } else {
      channel_1_prev = micros();
    }
  }
  if(x & 0x02) {
    if(previous_pink & 0x02) {
      channel_2 = micros() - channel_2_prev;
    } else {
      channel_2_prev = micros();
    }
  }
  if(x & 0x04) {
    if(previous_pink & 0x04) {
      channel_3 = micros() - channel_3_prev;
    } else {
      channel_3_prev = micros();
    }
  }
  if(x & 0x08) {
    if(previous_pink & 0x08) {
      channel_4 = micros() - channel_4_prev;
    } else {
      channel_4_prev = micros();
    }
  }
  if(x & 0x10) {
    if(previous_pink & 0x10) {
      channel_5 = micros() - channel_5_prev;
    } else {
      channel_5_prev = micros();
    }
  }
  if(x & 0x20) {
    if(previous_pink & 0x20) {
      channel_6 = micros() - channel_6_prev;
    } else {
      channel_6_prev = micros();
    }
  }
  if(x & 0x40) {
    if(previous_pink & 0x40) {
      channel_7 = micros() - channel_7_prev;
    } else {
      channel_7_prev = micros();
    }
  }
  previous_pink=PINK;
}

void process_rc_data() {
  smoothed_control_x = smoothed_control_x * 0.9 + (channel_1 - 1520) * CONTROL_SENSITIVITY * 0.1;
  smoothed_control_y = smoothed_control_y * 0.9 + (channel_2 - 1520) * CONTROL_SENSITIVITY * 0.1;
  smoothed_control_t = smoothed_control_t * 0.9 + (channel_3 - 1120) * 0.1;
  if(abs(channel_4 - 1520) > 30) {
    smoothed_control_z += (channel_4 - 1520) * CONTROL_SENSITIVITY_Z * 0.001;
    if(smoothed_control_z > 360.0) smoothed_control_z -= 360.0;
    if(smoothed_control_z < 0.0)   smoothed_control_z += 360.0;
  }
  armed = channel_5 > 1800;

  altitude_hold_control = altitude_hold_control * 0.9 + (channel_7 - 1120) * 0.1;
  if (altitude_hold_control < 0) altitude_hold_control = 0;
  if (altitude_hold_control > 1000) altitude_hold_control = 1000;
  previous_altitude_hold_control = altitude_hold_control;
  
  if(channel_6 > 1520) {
    if(initial_gps_lat && gps_online) {
      gps_enabled = 1;
    } else if (gps_online) {
      initial_gps_lat  = gps_lat;
      initial_gps_long = gps_long;
      gps_enabled = 1;
    } else {
      initial_gps_lat  = 0;
      initial_gps_long = 0;
      gps_enabled = 0;
    }
  } else {
    gps_enabled = 0;
    if(gps_online) {
      initial_gps_lat  = gps_lat;
      initial_gps_long = gps_long;
    } else {
      initial_gps_lat  = 0;
      initial_gps_long = 0;
    }
  }
}

