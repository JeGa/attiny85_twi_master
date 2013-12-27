/*
 * Functions for I2C-Master communication for attiny85 with the
 * USI-Module (TWI). Delays for SCL are used (no interrupts).
 *
 * All functions return 1 on success, 0 on failure.
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
#define WAIT_TIME ((1.0/TWI_CLOCK) * 1000 * 1000) // us

#define TWI_READ 1
#define TWI_WRITE 0

// For each Master <-> Client communication
typedef struct twi_connection {
    unsigned char client_address; // Without R/W bit
    unsigned char rw; // R/W bit
    unsigned char send_data; // Addr. and R/W ready to send in 1 Byte
} twi_connection_t;

// Prototypes
void init_twi(void);
int start_twi(twi_connection_t *con);
int stop_twi(void);
int send_twi(unsigned char *buffer, int size);

void init_twi_connection(twi_connection_t *con,
    unsigned char addr,
    unsigned char rw);

#endif