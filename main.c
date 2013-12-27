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

    if (!init_bp(&con)) {
        led_blink_error();
        stop_twi();
    } else {
        // Blink yeah
        for (i = 0; i < 10; ++i) {
            led_all_bp(&con, LED_ON);
            _delay_ms(500);
            led_all_bp(&con, LED_OFF);
            _delay_ms(500);
        }
    }

    while (1) {}
}