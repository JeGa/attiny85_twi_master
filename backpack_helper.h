/*
 * Helper functions for theLED backpack from Adafruit
 * (with HT16K33 controller). It uses the attiny85_twi_master
 * library for I2C communication.
 */

#ifndef BACKPACK_HELPER
#define BACKPACK_HELPER

#include "attiny85_twi_master.h"

#define LED_ON 0xFF
#define LED_OFF 0x0

int init_bp(twi_connection_t *con);
int led_all_bp(twi_connection_t *con, unsigned char state);
int led_row_bp(twi_connection_t *con,
    unsigned char row,
    unsigned char state);

#endif