#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

unsigned long int micros_counter=0;
ISR(TIMER0_OVF_vect)
{
  micros_counter += 1;
}

unsigned long int micros() {
  return((micros_counter * 256 + TCNT0)/2);
}

unsigned long int millis() {
  return(micros_counter / 7.8125);
}

void timers_init() {
  TCCR0A = 0x00;
  TCCR0B = 0x00;
  TCNT0 = 0x00;
  TIMSK0 = 0x01;
  TCCR0B |= (1 << CS11);
  
  TCCR1A = 0; // Set up high resolution timer1
  TCCR1B = 2; // Set up high resolution timer1
}
