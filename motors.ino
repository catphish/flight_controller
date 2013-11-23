// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Motor control functions

void set_velocities() {
  //Serial.print(roll_input, DEC);
  //Serial.print(", ");
  //Serial.print(pitch_input, DEC);
  //Serial.print(", ");
  //Serial.print(control_r, DEC);
  //Serial.print(", ");
  //Serial.print(control_x, DEC);
  //Serial.print(", ");
  //Serial.print(control_y, DEC);
  //Serial.print(", ");
  //Serial.println(control_z, DEC);

  //float ratio =  control_z / 700.0;
  int fl=1000 + smoothed_control_z + roll_output + pitch_output;// + control_r; // * ratio;
  int fr=1000 + smoothed_control_z - roll_output + pitch_output;// - control_r; // * ratio;
  int rl=1000 + smoothed_control_z + roll_output - pitch_output;// - control_r; // * ratio;
  int rr=1000 + smoothed_control_z - roll_output - pitch_output;// + control_r; // * ratio;
  
  timer = micros();
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
  //Serial.println(micros()-timer);
//  Serial.print(roll_input);
//  Serial.print(",");
//  Serial.println(pitch_input);
//  Serial.print(",");
//  Serial.print(roll_output_p + pitch_output_p);
//  Serial.print(",");
//  Serial.print(roll_output_i + pitch_output_i);
//  Serial.print(",");
//  Serial.println(roll_output_d + pitch_output_d);

  //if(fl > 1600) fl=1600;
  //if(fr > 1600) fr=1600;
  //if(rl > 1600) rl=1600;
  //if(rr > 1600) rr=1600;

//  front_left.writeMicroseconds(fl);
//  front_right.writeMicroseconds(fr);
//  rear_left.writeMicroseconds(rl);
//  rear_right.writeMicroseconds(rr);
  
//  Serial.print(control_x);
//  Serial.print("\t");
//  Serial.print(control_y);
//  Serial.print("\t");
//  Serial.print(roll_input);
//  Serial.print("\t");
//  Serial.print(pitch_input);
//  Serial.print("\t");
Serial.print(gyro_x + gyro_y);
Serial.print(",");
Serial.print(roll_input + pitch_input);
Serial.print(",");
Serial.print(roll_mid + pitch_mid);
Serial.print(",");
Serial.println(roll_output + pitch_output);
//  Serial.print("\t");
//  
//  Serial.print(fl, DEC);
//  Serial.print("\t");
//  Serial.print(fr, DEC);
//  Serial.print("\t");
//  Serial.print(rl, DEC);
//  Serial.print("\t");
//  Serial.println(rr, DEC);
}

