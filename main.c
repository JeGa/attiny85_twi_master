#include <avr/io.h>
#include <util/delay.h>
#include "attiny85_twi_master.h"
#include "trinket_helper.h"
#include "backpack_helper.h"

#define SLAVE_ADDRESS 0x70

unsigned char data[] = {
        0b11111111,
        0b01110111,
        0b11110101,
        0b01011101,
        0b11011101,
        0b11010101,
        0b01010101,
        0b01000101
};

unsigned char data2[] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
};
 
int main(void)
{
    twi_connection_t con;

    // Indicate start
    led_blink(1);

    // Initialization
    init_twi_connection(&con, SLAVE_ADDRESS, TWI_WRITE);
    init_twi();

    if (!init_bp(&con)) {
        led_blink_error();
        stop_twi();
    } else {
        // Blink yeah
        /*int i;
        for (i = 0; i < 5; ++i) {
            led_all_bp(&con, LED_ON);
            _delay_ms(200);
            led_all_bp(&con, LED_OFF);
            _delay_ms(200);
        }*/

        while (1) {
            for (int j = 0; j < 8; ++j) {
                led_row_bp(&con, j, data[j]);
                _delay_ms(100);
            }
            for (int j = 0; j < 8; ++j) {
                led_row_bp(&con, j, data2[j]);
                _delay_ms(100);
            }
            _delay_ms(200);
            led_all_bp(&con, LED_OFF);
        }
    }

    while (1) {}
}