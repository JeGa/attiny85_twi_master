/*
 * SDA can be set with SDA-reg or Port-reg. But SDA-reg. overwrites
 * Port-reg.
 *
 * 1 -> released state for SDA and SCL
 *
 * SDA muss immer nach manuellem own released werden wegen ver-UNDUNG!
 * Sons kann ueber USIDR nicht auf den Port zugegrifen werden.
 *
 */

#include "attiny85_twi_master.h"
#include <util/delay.h>

// Static prototypes
static int send_start_condition(void);
static int send_address(twi_connection_t *con);
//!!void send_twi_byte(unsigned char *byte);
static void wait_twi(void);
static void wait_twi_half(void);
static void clock_release(void);
static void clock_down(void);
static void data_release(void);
static void data_down(void);
static void reset_twi_status_register(void);

void init_twi(void)
{
    // Enable pullups
    PORT_TWI |= (1 << SDA) | (1 << SCL);

    // Set to out
    DDR_TWI |= (1 << SDA) | (1 << SCL);

    // Release SCL and SDA
    clock_release();
    data_release();
    USIDR = 0xFF;

    // Set the control register
    USICR = (0 << USISIE) | (0 << USIOIE) | // Disable interrupts
            (1 << USIWM1) | (0 << USIWM0) | // TW-Mode
            (0 << USICS1) | (0 << USICS0); // | (0 << USICLK); // Software strobe

    // Clear flags and reset counter
    reset_twi_status_register();
}

/*
 * Sends start condition and the slave address.
 */
unsigned char start_twi(twi_connection_t *con)
{
    if (!send_start_condition())
        return 0;

    return send_address(con);
}

/*
 * Sends stop condition.
 */
int stop_twi(twi_connection_t *con)
{
    // == Period start

    // Stop condition
    clock_release();
    data_release();
    wait_twi();

    // == Period end

    // Verify stop condition
    if (!(USISR & (1 << USIPF)))
        return 0;

    return 1;
}

void send_twi(void)
{

}

// ===================================================================

// Helper functions for twi_connection struct

void init_twi_connection(
    twi_connection_t *con,
    unsigned char addr,
    unsigned char rw)
{
    con->client_address = addr;
    con->rw = rw;
    con->send_data = (addr << 1) | (rw);
}

// Other helper functions

static int send_start_condition(void)
{
    _delay_ms(500);

    // == Period start

    // Start condition
    clock_release(); // Release SCL
    data_down(); // Pull SDA low
    wait_twi_half();

    clock_down(); // Pull SCL low
    data_release(); // Release SDA
    wait_twi_half();

    // == Period end

    // Verify start condition
    if (!(USISR & (1 << USISIF)))
        return 0;

    // Clear start condition flag and reset counter
    reset_twi_status_register();

    return 1;
}

static int send_address(twi_connection_t *con)
{
    unsigned char result;

    _delay_ms(500);

    send_twi_byte(&(con->send_data));

    // Get ACK =======================================================

    _delay_ms(500);

    // SDA as input
    DDR_TWI &= ~(1 << SDA);

    // Set 0 for inserting
    USIDR = 0x0;

    // == Period start

    clock_release();
    wait_twi_half();
    USICR |= (1 << USICLK); // Shift data register
    clock_down();
    wait_twi_half();

    // == Period end

    // Save the result
    result = USIDR;

    // release //!!
    USIDR = 0xFF;

    // SDA as output
    DDR_TWI |= (1 << SDA);

    //!! Check the result (Has to be 0!)
    if (result != 0)
        return 0;
    return 1;
}

// Sends one byte
void send_twi_byte(unsigned char *byte)
{
    int i;

    USIDR = *byte;

    for (i = 0; i < 8; ++i) {

        // == Period start

        clock_release();
        wait_twi_half();
        clock_down();
        wait_twi_half();

        // == Period end

        USICR |= (1 << USICLK); // Shift data register
    }

    // release //!!
    USIDR = 0xFF;

    reset_twi_status_register();

    // Check result ... //!! TODO
}

static void wait_twi(void)
{
    _delay_us(TWI_CLOCK);
}

static void wait_twi_half(void)
{
    _delay_us(TWI_CLOCK/2);
}

static void data_release(void)
{
    PORT_TWI |= (1 << SDA);
    //while (!(PIN_TWI & (1 << SDA))); // Wait until SDA is 1
}

static void data_down(void)
{
    PORT_TWI &= ~(1 << SDA);
    //while ((PIN_TWI & (1 << SDA))); // Wait until SDA is 0
}

static void clock_release(void)
{
    PORT_TWI |= (1 << SCL);
    while (!(PIN_TWI & (1 << SCL))); // Wait until SCL is 1
}

static void clock_down(void)
{
    PORT_TWI &= ~(1 << SCL);
    while ((PIN_TWI & (1 << SCL))); // Wait until SCL is 0
}

static void reset_twi_status_register(void)
{
    USISR = (1 << USISIF) | (1 << USIOIF) |
            (1 << USIPF) | (1 << USIDC) |
            (0x0 << USICNT0);
}