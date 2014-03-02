// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor output routines are here

#include "settings.h"
#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

void update_motors(unsigned char armed, int roll, int pitch, int yaw, int throttle) {
  int fl = 1064;
  int fr = 1064;
  int rl = 1064;
  int rr = 1064;
  if(armed) {
    // Calculate motor speeds
    fl = 1064 + roll - pitch - yaw + throttle;
    fr = 1064 - roll - pitch + yaw + throttle;
    rl = 1064 + roll + pitch + yaw + throttle;
    rr = 1064 - roll + pitch - yaw + throttle;
  }
  
  // Limit extremes to avoid confusing ESCs
  if(fl < 1064) fl = 1064;  if(fl > 2200) fl = 2200;
  if(fr < 1064) fr = 1064;  if(fr > 2200) fr = 2200;
  if(rl < 1064) rl = 1064;  if(rl > 2200) rl = 2200;
  if(rr < 1064) rr = 1064;  if(rr > 2200) rr = 2200;
  
  // Output pulses to ESCs simultaneously
  PORTB=240;
  TCNT1=0;
  while(PORTB & 240) {
    if(TCNT1 >= fl * 2)
      PORTB &= 224;
    if(TCNT1 >= fr * 2)
      PORTB &= 208;
    if(TCNT1 >= rl * 2)
      PORTB &= 176;
    if(TCNT1 >= rr * 2)
      PORTB &= 112;
  }
  
  // Pause to ensure there is always a gap between pulses
  _delay_us(5000);
}
