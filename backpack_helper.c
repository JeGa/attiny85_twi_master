#include "backpack_helper.h"
#include <util/delay.h>

// Commands for the LED controller
#define SYSTEM_SETUP 0b00100001
#define DISPLAY_SETUP 0b10000001

static int command_bp(twi_connection_t *con, unsigned char data);

int init_bp(twi_connection_t *con)
{
    if (!command_bp(con, SYSTEM_SETUP))
        return 0;

    _delay_ms(10);

    if (!command_bp(con, DISPLAY_SETUP))
        return 0;

    return 1;
}

int led_all_bp(twi_connection_t *con, unsigned char state)
{
        unsigned char addr = 0b00000000;
        int i;

        start_twi(con);
        send_twi(&addr , 1);

        for (i = 0; i < 16; ++i) {
            send_twi(&state, 1);
        }

        stop_twi();

        return 1; //!! TODO
}

int led_row_bp(twi_connection_t *con,
    unsigned char row,
    unsigned char state)
{
        unsigned char addr = row;
        int i;

        start_twi(con);
        send_twi(&addr , 1);
        send_twi(&state, 1);
        send_twi(&state, 1);
        stop_twi();

        return 1; //!! TODO
}

/*
 * One byte command.
 */
static int command_bp(twi_connection_t *con, unsigned char data)
{
    if (!start_twi(con))
        return 0;

    send_twi(&data, 1);
    stop_twi();

    return 1;
}