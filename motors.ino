// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor control functions

void set_velocities() {
  float ratio = smoothed_control_z / 700;
  int fl=1100 + (500 + roll_output + pitch_output) * ratio - gyro_z;
  int fr=1100 + (500 - roll_output + pitch_output) * ratio + gyro_z;
  int rl=1100 + (500 + roll_output - pitch_output) * ratio + gyro_z;
  int rr=1100 + (500 - roll_output - pitch_output) * ratio - gyro_z;
  
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
  
  //Serial.print(roll_input);
  //Serial.print(",");
  //Serial.print(pitch_input);
  //Serial.print(gyro_x + gyro_y);
  //Serial.print(",");
  //Serial.print(roll_mid + pitch_mid);
  //Serial.print(",");
  //Serial.print(roll_output + pitch_output);
  //Serial.print("\n");
  //Serial.println(fl);
}

