// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Interrupt driven radio receiver code

#include <avr/interrupt.h>
#include <avr/io.h>
#include "timers.h"

volatile long channel_1;
long channel_1_prev;
volatile long channel_2;
long channel_2_prev;
volatile long channel_3;
long channel_3_prev;
volatile long channel_4;
long channel_4_prev;
volatile long channel_5;
long channel_5_prev;
volatile long channel_6;
long channel_6_prev;
unsigned char previous_pink;

ISR(PCINT2_vect)
{
  unsigned char x;
  x = PINK^previous_pink;
  if(x & 0x01) {
    if(previous_pink & 0x01) {
      channel_1 = micros() - channel_1_prev;
    } else {
      channel_1_prev = micros();
    }
  }
  if(x & 0x02) {
    if(previous_pink & 0x02) {
      channel_2 = micros() - channel_2_prev;
    } else {
      channel_2_prev = micros();
    }
  }
  if(x & 0x04) {
    if(previous_pink & 0x04) {
      channel_3 = micros() - channel_3_prev;
    } else {
      channel_3_prev = micros();
    }
  }
  if(x & 0x08) {
    if(previous_pink & 0x08) {
      channel_4 = micros() - channel_4_prev;
    } else {
      channel_4_prev = micros();
    }
  }
  if(x & 0x10) {
    if(previous_pink & 0x10) {
      channel_5 = micros() - channel_5_prev;
    } else {
      channel_5_prev = micros();
    }
  }
  if(x & 0x20) {
    if(previous_pink & 0x20) {
      channel_6 = micros() - channel_6_prev;
    } else {
      channel_6_prev = micros();
    }
  }
  previous_pink=PINK;
}

void pci_init() {
  // Enable pin change interrupt on PORT K
  DDRK = 0x00;    // PORT K is INPUT
  PCMSK2 |= (1<<PCINT0)|(1<<PCINT1)|(1<<PCINT2)|(1<<PCINT3)|(1<<PCINT4)|(1<<PCINT5);
  PCICR  |= (1<<PCIE2);
}

