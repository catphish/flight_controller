#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

volatile unsigned long int micros_counter=0;

ISR(TIMER0_OVF_vect)
{
  micros_counter += 1;
}

unsigned long int micros() {
  unsigned long m, mm;
  cli();
  m = micros_counter;
  mm = TCNT0;
	if ((TIFR0 & _BV(TOV0)) && (mm < 255))
		m++;
  sei();
  
	m *= 256;
	m += mm;
	m /= 2;
  return m;
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
