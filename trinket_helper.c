#include "trinket_helper.h"
#include <util/delay.h>

static void set_ddr(void);

void led_on(void)
{
    set_ddr();
    LED_PORT |= (1 << LED_PIN);
}

void led_off(void)
{
    set_ddr();
    LED_PORT &= ~(1 << LED_PIN);
}

void led_blink(unsigned char nr)
{
    if (nr == 0)
        return;

    set_ddr();

    while (nr--) {
        LED_PORT |= (1 << LED_PIN);
        _delay_ms(500);
        LED_PORT &= ~(1 << LED_PIN);
        _delay_ms(500);
    }
}

void led_blink_error(void)
{
    int nr = ERROR_BLINK;
    set_ddr();

    while (nr--) {
        LED_PORT |= (1 << LED_PIN);
        _delay_ms(100);
        LED_PORT &= ~(1 << LED_PIN);
        _delay_ms(100);
    }
}

// DDR to out
static void set_ddr(void)
{
    LED_DDR |= (1 << LED_PIN);
}