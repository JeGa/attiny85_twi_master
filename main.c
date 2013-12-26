#include <avr/io.h>
#include "attiny85_twi_master.h"
#include "trinket_helper.h"

#include <util/delay.h>

#define SLAVE_ADDRESS 0x70
 
int main(void)
{
    twi_connection_t con;
    int res;
    unsigned char data;

    // Indicate start
    led_blink(5);

    init_twi_connection(&con, SLAVE_ADDRESS, TWI_WRITE);
    init_twi();


    if (!start_twi(&con))
        led_blink_error();
    //stop_twi(&con);


    _delay_ms(1000);


    data = 0b00001111;
    //send_twi_byte(&data);

    _delay_ms(300);

    data = 0b11111111;
    //send_twi_byte(&data);


    // Indicate stop
    //led_blink(res);

    while (1) {}
}