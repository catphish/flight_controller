// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// A basic set of UART operations

#include "settings.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>

void uart_putchar(char c) {
  if (c == '\n') {
    uart_putchar('\r');
  }
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}

void uart_putstring(char* string) {
  int n;
  for(n=0;n<strlen(string); n++) {
    uart_putchar(string[n]);
  }
}

void uart_putint(long number) {
  char buffer[64];
  itoa(number, buffer, 10);
  uart_putstring(buffer);
}

int uart_putchar_stream(char c, FILE *stream) {
  if (c == '\n') {
    uart_putchar('\r');
  }
  uart_putchar(c);
  return(0);
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar_stream, NULL, _FDEV_SETUP_WRITE);

void uart_init() {
  // Set up serial port
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
  
  stdout = &uart_output;
}
