// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Radio control interrupts and processing

unsigned char previous_pink=0;
unsigned char x=0;
long channel_1;
unsigned long channel_1_prev;
long channel_2;
unsigned long channel_2_prev;
long channel_3;
unsigned long channel_3_prev;
long channel_4;
unsigned long channel_4_prev;
long channel_5;
unsigned long channel_5_prev;
long channel_6;
unsigned long channel_6_prev;

double previous_altitude_hold_control;

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
  previous_pink=PINK;
}

void process_rc_data() {
  smoothed_control_x = smoothed_control_x * 0.9 + (channel_1 - 1500) * X_CONTROL_SENSITIVITY * 0.1;
  smoothed_control_y = smoothed_control_y * 0.9 + (channel_2 - 1500) * Y_CONTROL_SENSITIVITY * 0.1;
  smoothed_control_t = smoothed_control_t * 0.9 + (channel_3 - 1150) * 0.1;
  smoothed_control_z = smoothed_control_z * 0.9 + (channel_4 - 1500) * Z_CONTROL_SENSITIVITY * 0.1;
  armed = channel_5 > 1500;
  
  altitude_hold_control = altitude_hold_control * 0.9 + (channel_6 - 1000) * 0.1;
  if (altitude_hold_control < 0) altitude_hold_control = 0;
  if (altitude_hold_control > 1000) altitude_hold_control = 1000;
  if (altitude_hold_control >=1 and previous_altitude_hold_control < 1)
    set_pressure = pressure;
  previous_altitude_hold_control = altitude_hold_control;
  
  // Don't engage altitude control unless it's been zero'd
  if (set_pressure < 1) altitude_hold_control = 0;
}

