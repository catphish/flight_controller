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
  PORTA=15;
  TCNT1=0;
  while(PORTA & 15) {
    if(TCNT1 >= fl * 2)
      PORTA &= 14;
    if(TCNT1 >= fr * 2)
      PORTA &= 13;
    if(TCNT1 >= rl * 2)
      PORTA &= 11;
    if(TCNT1 >= rr * 2)
      PORTA &= 7;
  }
  
  // Pause to ensure there is always a gap between pulses
  _delay_us(5000);
}
