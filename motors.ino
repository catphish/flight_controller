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
  int fl=1064 + (400 + output_x - output_y + output_z + altitude_hold_correction) * 0.5 * armed + 600 * ratio;
  int fr=1064 + (400 - output_x - output_y - output_z + altitude_hold_correction) * 0.5 * armed + 600 * ratio;
  int rl=1064 + (400 + output_x + output_y - output_z + altitude_hold_correction) * 0.5 * armed + 600 * ratio;
  int rr=1064 + (400 - output_x + output_y + output_z + altitude_hold_correction) * 0.5 * armed + 600 * ratio;
  
  // Limit extremes to avoid confusing ESCs
  if(fl < 1064) fl = 1064;  if(fl > 2200) fl = 2200;
  if(fr < 1064) fr = 1064;  if(fr > 2200) fr = 2200;
  if(rl < 1064) rl = 1064;  if(rl > 2200) rl = 2200;
  if(rr < 1064) rr = 1064;  if(rr > 2200) rr = 2200;
  
  // ESC Calibration
  //fl = fr = rl = rr = channel_3;
  
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


