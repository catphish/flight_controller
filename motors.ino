// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor control functions
float ratio;

void set_velocities() {
  if(armed) {
    ratio = smoothed_control_t / 730;
  } else {
    ratio = 0;
  }
  int fl=1064 + 100 * armed + (500 + output_x - output_y - output_z) * ratio;
  int fr=1064 + 100 * armed + (500 - output_x - output_y + output_z) * ratio;
  int rl=1064 + 100 * armed + (500 + output_x + output_y + output_z) * ratio;
  int rr=1064 + 100 * armed + (500 - output_x + output_y - output_z) * ratio;
  
  PORTB=16;
  TCNT1=0;
  while(TCNT1 < fl * 2);
  PORTB=32;
  TCNT1=0;
  while(TCNT1 < fr * 2);
  PORTB=64;
  TCNT1=0;
  while(TCNT1 < rl * 2);
  PORTB=128;
  TCNT1=0;
  while(TCNT1 < rr * 2);
  PORTB=0;
}

