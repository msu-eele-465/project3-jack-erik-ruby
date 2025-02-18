/**
* @file
* @brief Uses PWMs through 3 arbitrary outputs to change an LED's color
*
* Assumes all 3 ports are on the same base.
*/
#include <msp430.h>

#include "led_status.h"

#include <stdio.h>

/**
* initializes LED and starts the timer compares
*
* Uses states to determine correct color to set LED
* pulses LED at correct % (x/255) to attain the color
* Uses Timer TB3: CCR1, 2, and 3
* 
* @param: sl constructor.
* @param: s current state.
*
* @return: NA
*/
void init_LED(status_LED sl)
{
    
    // Define the base addresses for the port multiplexing registers, PxSEL1 and
    // PxSEL0. These offsets are NOT the same for all ports, and are defined in
    // msp430fr2355.h.
    uint16_t *PX_SEL0 = (uint16_t *) (sl.led_port_base_addr + OFS_P6SEL0); 
    uint16_t *PX_DIR = (uint16_t *) (sl.led_port_base_addr + OFS_P6DIR);

     // Configure GPIO
    *PX_DIR |= sl.red_port_bit + sl.green_port_bit + sl.blue_port_bit;
    *PX_SEL0 |= sl.red_port_bit + sl.green_port_bit + sl.blue_port_bit;
    // printf("PX_SEL0 address: %d\n", *PX_SEL0);
    // printf("PX_DIR address: %d\n", *PX_DIR);

    // P6DIR |= sl.red_port_bit + sl.green_port_bit + sl.blue_port_bit;
    // P6SEL0 |= sl.red_port_bit + sl.green_port_bit + sl.blue_port_bit;

    TB3CCR0 = 256-1;                          // PWM Period
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set (red)
    TB3CCTL2 = OUTMOD_7;                      // CCR2 reset/set (green)
    TB3CCTL3 = OUTMOD_7;                      // CCR3 reset/set (blue)
    // start locked
    TB3CCR1 = 196;                              // CCR1 PWM duty cycle: Amount of red 
    TB3CCR2 = 62;                               // CCR2 PWM duty cycle: Amount of green 
    TB3CCR3 = 29;                               // CCR3 PWM duty cycle: Amount of blue
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR

    PM5CTL0 &= ~LOCKLPM5;   // turn on GPIO 
}

/**
* Sets LEDs with right colors
*
* Uses states to determine correct color to set LED
* pulses LED at correct % (x/255) to attain the color
* 
* @param: sL constructor.
* @param: s current state.
*
* @return: NA
*/
void set_LED(status_LED *sl, LED_State s)
{
    sl->current_state = s;
    TB3CTL &= ~MC__UP;  // setting MC=0 to stop timer

    // Count up to deltaT, then to T-deltaT, then to T-that, etc.
   switch (s) 
   {
        case LOCKED:        // set pwm to rgb(196 62 29)
            TB3CCR1 = 196;  // CCR1 PWM duty cycle: Amount of red 
            TB3CCR2 = 62;   // CCR2 PWM duty cycle: Amount of green 
            TB3CCR3 = 29;   // CCR3 PWM duty cycle: Amount of blue
            break;

        case MIDUNLOCK:     // rgb(196 146 29)
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29;   
            break;

        case UNLOCKED:      // rgb(29 162 196)
            TB3CCR1 = 29;  
            TB3CCR2 = 162;   
            TB3CCR3 = 196;   
            break;

        case PATTERN0:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29;  
            break;

        case PATTERN1:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29; 
            break;

        case PATTERN2:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29; 
            break;

        case PATTERN3:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29;   
            break;

        case PATTERN4:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29;   
            break;

        case PATTERN5:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29;   
            break;

        case PATTERN6:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29;   
            break;

        case PATTERN7:
            TB3CCR1 = 196;  
            TB3CCR2 = 146;   
            TB3CCR3 = 29;   
            break;

        // default to locked
        default:
            TB3CCR1 = 196;  // CCR1 PWM duty cycle: Amount of red 
            TB3CCR2 = 62;   // CCR2 PWM duty cycle: Amount of green 
            TB3CCR3 = 29;   // CCR3 PWM duty cycle: Amount of blue
            break;
    }
    TB3CTL |= MC__UP;  // start timer again in up mode
}
