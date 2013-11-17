// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor control functions

void set_velocities() {
  int fl=1000 + control_z + control_x - x - control_y - y + control_r;
  int fr=1000 + control_z - control_x + x - control_y - y - control_r;
  int rl=1000 + control_z + control_x - x + control_y + y - control_r;
  int rr=1000 + control_z - control_x + x + control_y + y + control_r;
  
  if(fl > 1500) fl=1500;
  if(fr > 1500) fr=1500;
  if(rl > 1500) rl=1500;
  if(rr > 1500) rr=1500;
  
  front_left.writeMicroseconds(fl);
  front_right.writeMicroseconds(fr);
  rear_left.writeMicroseconds(rl);
  rear_right.writeMicroseconds(rr);
  
  //Serial.print(fl, DEC);
  //Serial.print(", ");
  //Serial.print(fr, DEC);
  //Serial.print(", ");
  //Serial.print(rl, DEC);
  //Serial.print(", ");
  //Serial.println(rr, DEC);
}

