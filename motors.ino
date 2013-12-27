// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor control functions
float ratio;

void set_velocities() {
  // Ignition
  if(armed) {
    ratio = smoothed_control_t / 730;
  } else {
    ratio = 0;
  }
  
  // Calculate motor speeds
  int fl=1064 + (400 + output_x - output_y - output_z + altitude_hold_correction) * 0.5 * armed + 400 * ratio;
  int fr=1064 + (400 - output_x - output_y + output_z + altitude_hold_correction) * 0.5 * armed + 400 * ratio;
  int rl=1064 + (400 + output_x + output_y + output_z + altitude_hold_correction) * 0.5 * armed + 400 * ratio;
  int rr=1064 + (400 - output_x + output_y - output_z + altitude_hold_correction) * 0.5 * armed + 400 * ratio;
  
  // Output pulses to ESCs simultaneously
  PORTB=240;
  TCNT1=0;
  while(PORTB & 240) {
    if(TCNT1 >= fl * 2)
      PORTB &= 239;
    if(TCNT1 >= fr * 2)
      PORTB &= 223;
    if(TCNT1 >= rl * 2)
      PORTB &= 191;
    if(TCNT1 >= rr * 2)
      PORTB &= 127;
  }
  // Pause to ensure there is always a gap between pulses
  delayMicroseconds(200);
}


