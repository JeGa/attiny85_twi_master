#include <avr/io.h>
#include <util/delay.h>
#include "attiny85_twi_master.h"
#include "trinket_helper.h"
#include "backpack_helper.h"

#define SLAVE_ADDRESS 0x70
 
int main(void)
{
    twi_connection_t con;
    unsigned char data;
    int i;

    // Indicate start
    led_blink(5);

    // Initialization
    init_twi_connection(&con, SLAVE_ADDRESS, TWI_WRITE);
    init_twi();

    if (!init_bp(&con))
        led_blink_error();
    else {
        // Send start addr.
        start_twi(&con);
        _delay_ms(10);
        data = 0b00000000;
        send_twi(&data, 1);

        for (i = 0; i < 16; ++i) {
            data = 0b11100111;
            send_twi(&data, 1);
        }

        stop_twi();

        for (i = 0; i < 5; ++i) {
        start_twi(&con);
        data = 0b00000000;
        send_twi(&data, 1);
        data = i+2;
        send_twi(&data, 1);
        data = i+1;
        send_twi(&data, 1);
        stop_twi();
        }


    }

    stop_twi();

    while (1) {}
}