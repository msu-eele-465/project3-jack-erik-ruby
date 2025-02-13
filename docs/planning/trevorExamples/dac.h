#ifndef DAC_H
#define DAC_H

#include <stdint.h>

#define DAC_N_BITS 12

#define DAC_VOLTAGE_REF_MV 1500

struct dac_t {
    const uint16_t sac_base_addr;
    const uint16_t port_base_addr;
    const uint16_t port_bit;
    uint16_t data;
};

void init_dac(struct dac_t dac);

// sets voltage (input in mV) to DAC
void set_dac_voltage(struct dac_t *dac, uint16_t voltage);

#endif // DAC_H
