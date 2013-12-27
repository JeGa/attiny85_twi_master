/*
 * SDA can be set with USIDR or PORTB register. However,
 * be careful because SDA ans SCL are open collector outputs.
 * -> This means the bus is a logical AND!
 *
 * 1 -> released state for SDA and SCL.
 * SDA has to be released after each pull.
 */

#include "attiny85_twi_master.h"
#include <util/delay.h>

// Static prototypes
static int send_start_condition(void);
static int send_twi_byte(unsigned char *byte);
static int get_ack(void);

//static void wait_twi(void);
static void wait_twi_half(void);
static void clock_release(void);
static void clock_down(void);
static void data_release(void);
static void data_down(void);
static void reset_twi_status_register(void);

/*
 * Initialzes the TWI registers.
 */
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
            (0 << USICS1) | (0 << USICS0); // | (0 << USICLK); // Software strobe //!! TODO

    // Clear flags and reset counter
    reset_twi_status_register();
}

/*
 * Initializes the connection struct.
 */
void init_twi_connection(twi_connection_t *con,
    unsigned char addr,
    unsigned char rw)
{
    con->client_address = addr;
    con->rw = rw;
    con->send_data = (addr << 1) | (rw);
}

/*
 * Sends start condition and the slave address.
 */
int start_twi(twi_connection_t *con)
{
    if (!send_start_condition())
        return 0;

    // Send the slave address with R/W Bit
    return send_twi_byte(&(con->send_data));
}

/*
 * Sends stop condition.
 */
int stop_twi(void)
{
    // == Period start

    data_down();
    wait_twi_half();

    clock_release();
    wait_twi_half();
    data_release();

    // == Period end

    // Verify stop condition
    if (!(USISR & (1 << USIPF)))
        return 0;

    reset_twi_status_register();

    return 1;
}

/*
 * Sends the data in the buffer (with size "size")s over the I2C bus.
 * (Needs previous called start_twi()).
 */
int send_twi(unsigned char *buffer, int size)
{
    int i, result;

    if (size <= 0)
        return 0;

    for (i = 0; i < size; ++i) {
        result = send_twi_byte(&buffer[i]);

        if (!result) {
            // Failure
            break;
        }
    }
    return result;
}

// ===================================================================

// Other helper functions

static int send_start_condition(void)
{
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

// Sends one byte and checks ack.
static int send_twi_byte(unsigned char *byte)
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

    // Release SDA
    USIDR = 0xFF;

    reset_twi_status_register();

    //_delay_ms(10);

    return get_ack();
}

static int get_ack(void)
{
    unsigned char result;

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

    // Release SDA
    USIDR = 0xFF;

    // SDA as output
    DDR_TWI |= (1 << SDA);

    // Check result
    if (result != 0)
        return 0;
    return 1;
}

/*
 * Unused.

static void wait_twi(void)
{
    _delay_us(WAIT_TIME);
}

 */

static void wait_twi_half(void)
{
    _delay_us(WAIT_TIME/2.0);
}

static void data_release(void)
{
    PORT_TWI |= (1 << SDA);
}

static void data_down(void)
{
    PORT_TWI &= ~(1 << SDA);
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