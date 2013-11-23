// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Radio control interrupts

long rc0_micros=0;
void readrc0() {
  if ( PINE & 0x10 ) {
    control_x = micros() - rc0_micros;
  } else {
    rc0_micros = micros();
  }
}
long rc1_micros=0;
void readrc1() {
  if ( PINE & 0x20 ) {
    control_y = micros() - rc1_micros;
  } else {
    rc1_micros = micros();
  }
}
long rc2_micros=0;
void readrc2() {
  if ( PIND & 0x04 ) {
    control_z = micros() - rc2_micros;
  } else {
    rc2_micros = micros();
  }
}
long rc3_micros=0;
void readrc3() {
  if ( PIND & 0x08 ) {
    control_r = micros() - rc3_micros;
  } else {
    rc3_micros = micros();
  }
}

