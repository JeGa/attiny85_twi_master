/*
 * Functions for I2C-Master communication for attiny85 with the
 * USI-Module (TWI). Delays for SCL are used (no interrupts).
 */

#ifndef ATTINY85_TWM_MASTER_H
#define ATTINY85_TWM_MASTER_H

#include <avr/io.h>

// Ports (for other devices change here)
#define SDA PB0
#define SCL PB2
#define PORT_TWI PORTB
#define PIN_TWI PINB
#define DDR_TWI DDRB

// Clock
#define TWI_CLOCK 100000 // Hz

//  Standard mode (not a very precise calculation)
#define WAIT_TIME ((1/TWI_CLOCK) * 1000000) * 2 // us

#define TWI_READ 1
#define TWI_WRITE 0

// For each Master <-> Client communication
typedef struct twi_connection {
    unsigned char client_address; // Without R/W bit
    unsigned char rw; // R/W bit
    unsigned char send_data; // addr. and rw ready to send in 1 Byte
} twi_connection_t;

// Prototypes
void init_twi(void);
void init_twi_connection(
    twi_connection_t *con,
    unsigned char addr,
    unsigned char rw);

int start_twi(twi_connection_t *con);
int stop_twi(void);
void send_twi(unsigned char *buffer, int size);

#endif