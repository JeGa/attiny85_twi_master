/*
 * Helper functions for theLED backpack from Adafruit
 * (with HT16K33 controller). It uses the attiny85_twi_master
 * library for I2C communication.
 */

#ifndef BACKPACK_HELPER
#define BACKPACK_HELPER

#include "attiny85_twi_master.h"

int init_bp(twi_connection_t *con);

#endif