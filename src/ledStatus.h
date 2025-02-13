#ifndef LED_H
#define LED_H

#include <stdint.h>

// r/255 = %on/total period
// #define DAC_N_BITS 12

// #define DAC_VOLTAGE_REF_MV 1500

enum State {
    LOCKED,
    MIDUNLOCK,
    UNLOCKED,
    PATTERN0,
    PATTERN1,
    PATTERN2,
    PATTERN3,
    PATTERN4,
    PATTERN5,
    PATTERN6,
    PATTERN7
};

struct status_LED { // each module has base address, port 2 base address. FInd in msp4340 header
// direction register will be certain offset
// port 2 direction, out, etc.
// check out pwm mode datasheet
    const uint16_t red_port_base_addr;
    const uint16_t red_port_bit;
    State currentState;
};

void init_LED(struct status_LED sL);

// sets voltage (input in mV) to DAC
void set_LED(struct status_LED *sL, State cs);

#endif // LED_H