#include <avr/io.h>
#include <util/delay.h>
#include "attiny85_twi_master.h"
#include "trinket_helper.h"

#define SLAVE_ADDRESS 0x70
 
int main(void)
{
    twi_connection_t con;
    unsigned char data;
    int i;

    // Indicate start
    led_blink(5);

    init_twi_connection(&con, SLAVE_ADDRESS, TWI_WRITE);
    init_twi();

    if (!start_twi(&con))
        led_blink_error();
    else {
        // System set
        _delay_ms(10);
        data = 0b00100001;
        send_twi(&data, 1);
        stop_twi();

        _delay_ms(10);
        start_twi(&con);
        // Display setup
        data = 0b10000001;
        send_twi(&data, 1);
        stop_twi();


        start_twi(&con);
        // Send start addr.
        _delay_ms(10);
        data = 0b00000000;
        send_twi(&data, 1);

        // All to 0
        for (i = 0; i < 16; ++i) {
            data = 0b10010111;
            send_twi(&data, 1);
        }

        stop_twi();
    }

    stop_twi();

    while (1) {}
}