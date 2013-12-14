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
  // TODO: Compensate x/y with additional throttle to revent loss of height when maneuvering
  int fl=1064 + (150 + output_x - output_y - output_z + altitude_hold_correction) * armed + 400 * ratio;
  int fr=1064 + (150 - output_x - output_y + output_z + altitude_hold_correction) * armed + 400 * ratio;
  int rl=1064 + (150 + output_x + output_y + output_z + altitude_hold_correction) * armed + 400 * ratio;
  int rr=1064 + (150 - output_x + output_y - output_z + altitude_hold_correction) * armed + 400 * ratio;
  
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
  delayMicroseconds(200);
}


