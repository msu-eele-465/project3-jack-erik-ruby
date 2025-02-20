#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "pmt_config.h"

// TODO: better name?
// TODO: move to pmt_config.h?
typedef struct {
    pmt_type pmt;
    uint16_t data;
} pmt_data_t;

bool validate_format(const char *input);

bool validate_voltage_bounds(const uint16_t voltage, const uint16_t upper_bound, const uint16_t lower_bound);

bool parse_command(const char *input, pmt_data_t *parsed);

#endif // PARSER_H