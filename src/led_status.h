/**
* @file
* @brief Header file for led_status.c
*
*/
#ifndef LED_H
#define LED_H

#include <stdint.h>

typedef enum
{
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
} LED_State;

/**
* holds port addresses and bits for RGB outputs plus current state
*/
typedef struct status_LED 
{ 
    const uint16_t led_port_base_addr;
    const uint16_t red_port_bit;
    const uint16_t green_port_bit;
    const uint16_t blue_port_bit;
    LED_State current_state;
} status_LED;

void init_LED(status_LED sL);

void set_LED(status_LED *sL, LED_State s);

#endif // LED_H
