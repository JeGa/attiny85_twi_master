/*
 * Helper functions for the adafruit trinket arduino.
 */

#ifndef TRINKET_HELPER
#define TRINKET_HELPER

#include <avr/io.h>

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PB1

#define ERROR_BLINK 10

void led_on(void);
void led_off(void);
void led_blink(unsigned char nr);
void led_blink_error(void);

#endif