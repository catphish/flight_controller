#include <avr/interrupt.h>
#include <avr/io.h>
#include "timers.h"

unsigned long channel_1;
unsigned long channel_1_prev;
unsigned long channel_2;
unsigned long channel_2_prev;
unsigned long channel_3;
unsigned long channel_3_prev;
unsigned long channel_4;
unsigned long channel_4_prev;
unsigned long channel_5;
unsigned long channel_5_prev;
unsigned long channel_6;
unsigned long channel_6_prev;
unsigned char previous_pina;

ISR(PCINT0_vect)
{
  unsigned char x;
  x = PINA^previous_pina;
  if(x & 0x01) {
    if(previous_pina & 0x01) {
      channel_1 = micros() - channel_1_prev;
    } else {
      channel_1_prev = micros();
    }
  }
  if(x & 0x02) {
    if(previous_pina & 0x02) {
      channel_2 = micros() - channel_2_prev;
    } else {
      channel_2_prev = micros();
    }
  }
  if(x & 0x04) {
    if(previous_pina & 0x04) {
      channel_3 = micros() - channel_3_prev;
    } else {
      channel_3_prev = micros();
    }
  }
  if(x & 0x08) {
    if(previous_pina & 0x08) {
      channel_4 = micros() - channel_4_prev;
    } else {
      channel_4_prev = micros();
    }
  }
  if(x & 0x10) {
    if(previous_pina & 0x10) {
      channel_5 = micros() - channel_5_prev;
    } else {
      channel_5_prev = micros();
    }
  }
  if(x & 0x20) {
    if(previous_pina & 0x20) {
      channel_6 = micros() - channel_6_prev;
    } else {
      channel_6_prev = micros();
    }
  }
  previous_pina=PINA;
}

void pci_init() {
  // Enable pin change interrupt on PORT A
  PCICR  |= (1<<PCIE0);
  PCMSK0 = 0b00111111;   // Mask PIN 0-5
}

