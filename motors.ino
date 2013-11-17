// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor control functions

void set_velocities() {
  //Serial.print(x, DEC);
  //Serial.print(", ");
  //Serial.print(y, DEC);
  //Serial.print(", ");
  //Serial.print(control_r, DEC);
  //Serial.print(", ");
  //Serial.print(control_x, DEC);
  //Serial.print(", ");
  //Serial.print(control_y, DEC);
  //Serial.print(", ");
  //Serial.println(control_z, DEC);

  //control_z = 700;
  //control_x = control_y = 0;
  
  float ratio =  control_z / 700.0;
  int fl=900 + control_z + (0 + control_x - x - control_y - y + control_r) * ratio;
  int fr=900 + control_z + (0 - control_x + x - control_y - y - control_r) * ratio;
  int rl=900 + control_z + (0 + control_x - x + control_y + y - control_r) * ratio;
  int rr=900 + control_z + (0 - control_x + x + control_y + y + control_r) * ratio;
  
  if(fl > 1600) fl=1600;
  if(fr > 1600) fr=1600;
  if(rl > 1600) rl=1600;
  if(rr > 1600) rr=1600;
  rl=0;
  rr=0;
  front_left.writeMicroseconds(fl);
  front_right.writeMicroseconds(fr);
  rear_left.writeMicroseconds(rl);
  rear_right.writeMicroseconds(rr);
  
  //Serial.print(x, DEC);
  //Serial.print("\t");
  //Serial.print(y, DEC);
  //Serial.print("\t");
  //Serial.print(fl, DEC);
  //Serial.print("\t");
  //Serial.print(fr, DEC);
  //Serial.print("\t");
  //Serial.print(rl, DEC);
  //Serial.print("\t");
  //Serial.println(rr, DEC);
}

