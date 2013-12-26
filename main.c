#include <avr/io.h>
#include <util/delay.h>
#include "attiny85_twi_master.h"
#include "trinket_helper.h"

#define SLAVE_ADDRESS 0x70
 
int main(void)
{
    twi_connection_t con;
    unsigned char data;

    // Indicate start
    led_blink(5);

    init_twi_connection(&con, SLAVE_ADDRESS, TWI_WRITE);
    init_twi();

    if (!start_twi(&con))
        led_blink_error();
    else {
        _delay_ms(300);
        data = 0b11111111;
        send_twi(&data, 1);
        _delay_ms(300);
        data = 0b11111111;
        send_twi(&data, 1);
        _delay_ms(300);
        data = 0b11111111;
        send_twi(&data, 1);
        _delay_ms(300);
        data = 0b11111111;
        send_twi(&data, 1);
    }

    _delay_ms(1000);
    stop_twi();

    while (1) {}
}