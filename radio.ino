// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Radio control interrupts and processing

long rc0_micros=0;
void readrc0() {
  if ( PINE & 0x10 ) {
    rc0_micros = micros();
  } else {
    control_x = micros() - rc0_micros;
  }
}
long rc1_micros=0;
void readrc1() {
  if ( PINE & 0x20 ) {
    rc1_micros = micros();
  } else {
    control_y = micros() - rc1_micros;
  }
}
long rc2_micros=0;
void readrc2() {
  if ( PIND & 0x08 ) {
    rc2_micros = micros();
  } else {
    control_t = micros() - rc2_micros;
  }
}
long rc3_micros=0;
void readrc3() {
  if ( PIND & 0x04 ) {
    rc3_micros = micros();
  } else {
    control_z = micros() - rc3_micros;
  }
}
void process_rc_data() {
  smoothed_control_x = smoothed_control_x * 0.9 + (control_x - 1500) * X_CONTROL_SENSITIVITY * 0.1;
  smoothed_control_y = smoothed_control_y * 0.9 + (control_y - 1520) * Y_CONTROL_SENSITIVITY * 0.1;
  smoothed_control_t = smoothed_control_t * 0.9 + (control_t - 1150) * 0.1;
  smoothed_control_z = smoothed_control_z * 0.9 + (control_z - 1500) * Z_CONTROL_SENSITIVITY * 0.1;
}

