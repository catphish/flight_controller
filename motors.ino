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
  int fl=1064 + 150 * armed + (400 + output_x - output_y - output_z) * ratio + altitude_hold_correction;
  int fr=1064 + 150 * armed + (400 - output_x - output_y + output_z) * ratio + altitude_hold_correction;
  int rl=1064 + 150 * armed + (400 + output_x + output_y + output_z) * ratio + altitude_hold_correction;
  int rr=1064 + 150 * armed + (400 - output_x + output_y - output_z) * ratio + altitude_hold_correction;
  
  // Output pulses to ESCs
  // TODO: Update all 4 ESCs at the same time
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


