// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Radio control interrupts

long control_x_center=-1, control_y_center=-1, control_r_center=-1, control_z_center=-1;
int control_x_count = 0, control_y_count = 0, control_z_count = 0, control_r_count = 0;

long rc0_micros=0L;
void readrc0() {
  if ( digitalRead(2) == HIGH ) {
    rc0_micros = micros();
  } else if(rc0_micros > 0) {
    long t = micros() - rc0_micros;
    if(t > 900 and t < 2200) {
      if(control_x_center ==-1L) {
        control_x_center = t;
      } else if (control_x_count < 10){
        control_x_center = control_x_center * 0.9 + t * 0.1;
        control_x_count++;
      } else {
        control_x = control_x * 0.9 + (t - control_x_center) * X_CONTROL_SENSITIVITY * 0.1;
      }
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
      if(control_y_center ==-1L) {
        control_y_center = t;
      } else if (control_y_count < 10){
        control_y_center = control_y_center * 0.9 + t * 0.1;
        control_y_count++;
      } else {
        control_y = control_y * 0.9 + (t - control_y_center) * Y_CONTROL_SENSITIVITY * 0.1;
      }
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
      if(control_z_center ==-1L) {
        control_z_center = t;
      } else if (control_z_count < 10){
        control_z_center = control_z_center * 0.9 + t * 0.1;
        control_z_count++;
      } else {
        control_z = t - control_z_center;
      }
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
      if(control_r_center ==-1L) {
        control_r_center = t;
      } else if (control_r_count < 10){
        control_r_center = control_r_center * 0.9 + t * 0.1;
        control_r_count++;
      }
      control_r = control_r * 0.9 + (t - control_r_center) * R_CONTROL_SENSITIVITY * 0.1;

    }
  }
}


