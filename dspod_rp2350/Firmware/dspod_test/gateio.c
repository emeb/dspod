/*
 * gateio.h - gate IO driver for dspod
 * 08-28-25 E. Brombaugh
 */
#include "gateio.h"

#define GATE_IN_PIN 14
#define GATE_OUT_PIN 15

// Initialize the GPIO for the Gate
void gate_init(void)
{
    gpio_init(GATE_OUT_PIN);
    gpio_set_dir(GATE_OUT_PIN, GPIO_OUT);

}

// Turn the Gate Out on or off
void set_gate(bool gate_on)
{
    // Just set the GPIO on or off
    gpio_put(GATE_OUT_PIN, gate_on);
}

