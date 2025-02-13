#ifndef PMT_CONFIG_H
#define PMT_CONFIG_H

#include <stdint.h>

typedef enum {
    SIGNAL,
    TRIGGER
} pmt_type;

typedef struct
{
    // PMT type from the pmt_type enum
    const pmt_type pmt;
    // control voltage upper limit, in mV
    const uint16_t control_voltage_upper_bound;
    // control voltage lower limit, in mV
    const uint16_t control_voltage_lower_bound;
} pmt_info_t;

static const pmt_info_t pmts[] = 
{
    // Output signal PMT info
    [SIGNAL] = {
        .pmt = SIGNAL,
        .control_voltage_upper_bound = 1000,
        .control_voltage_lower_bound = 250,
    },
    // Trigger PMT info
    [TRIGGER] = {
        .pmt = TRIGGER,
        .control_voltage_upper_bound = 900,
        .control_voltage_lower_bound = 250,
    }
};

#endif // PMT_CONFIG_H