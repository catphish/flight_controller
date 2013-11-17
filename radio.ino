// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Radio control interrupts

long rc0_micros=0L;
void readrc0() {
  if ( digitalRead(2) == HIGH ) {
    rc0_micros = micros();
  } else if(rc0_micros > 0) {
    long t = micros() - rc0_micros;
    if(t > 900 and t < 2200) {
      if(control_x_center ==-1L) 
        control_x_center = t;
      control_x = (t - control_x_center) * X_CONTROL_SENSITIVITY;
    }
  }
}

long rc1_micros=0L;
void readrc1() {
  if ( digitalRead(3) == HIGH ) {
    rc1_micros = micros();
  } else if(rc1_micros > 0) {
    long t = micros() - rc1_micros;
    if(t > 900 and t < 2200) {
      if(control_y_center ==-1L) 
        control_y_center = t;
      control_y = (t - control_y_center) * Y_CONTROL_SENSITIVITY;
    }
  }
}

long rc2_micros=0L;
void readrc2() {
  if ( digitalRead(18) == HIGH ) {
    rc2_micros = micros();
  } else if(rc2_micros > 0) {
    long t = micros() - rc2_micros;
    if(t > 900 and t < 2200) {
      if(control_z_center ==-1L) 
        control_z_center = t;
      control_z = t - control_z_center;
    }
  }
}

long rc3_micros=0L;
void readrc3() {
  if ( digitalRead(19) == HIGH ) {
    rc3_micros = micros();
  } else if(rc3_micros > 0) {
    long t = micros() - rc3_micros;
    if(t > 900 and t < 2200) {
      if(control_r_center ==-1L) 
        control_r_center = t;
      control_r = t - control_r_center;

    }
  }
}

