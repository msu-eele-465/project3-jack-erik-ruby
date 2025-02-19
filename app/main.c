/*
 * Erik Callery, EELE371, Lab 13.2
 * Last modified: March 6 2024 EC
 */
#include <msp430.h> 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../src/keypad.h"
#include "../src/led_status.h"
#include "msp430fr2355.h"


uint16_t base_transition_period;
uint16_t current_pattern;
// starting values for every pattern
uint16_t default_patterns[8] = {170, 170, 0, 24, 255, 1, 127, 1};
uint16_t patterns[8] = {170, 170, 0, 24, 255, 1, 127, 1};;
char cur_char;

// #pragma PERSISTENT stores these variables in FRAM so they persist across
// power cycles. This way the DAC voltages can be set once, rather than having
// to set them every time we power on the system. 5.0 to 5.2
__attribute__((persistent)) static status_LED status_led =
{
    .led_port_base_addr = P5_BASE,
    .red_port_bit = BIT0,
    .green_port_bit = BIT1,
    .blue_port_bit = BIT2,
    .current_state = LEDLOCKED
};


void init_unused(void)
{
    //this just goes through every pin that is not
    //directly necessary, and puts it to output, pulled low

    // P1DIR |= BIT5 | BIT4 | BIT3 | BIT2 | BIT1;
    // P1OUT &= ~(BIT5 | BIT4 | BIT3 | BIT2 | BIT1);

    // P2DIR |= BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    // P2OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

    // P3DIR |= BIT7 | BIT6 | BIT4 | BIT3 | BIT2 |BIT0;
    // P3OUT &= ~(BIT7 | BIT6 | BIT4 | BIT3 | BIT2 | BIT0);

    // P4DIR |= BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    // P4OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

    // P5DIR |= BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    // P5OUT &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

    // P6DIR |= BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    // P6OUT &= ~(BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
}


void init(void)
{
    // Disable watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

//------------- Setup Ports --------------------
    // LED1
    P1DIR |= BIT0;          // Config as Output
    P1OUT |= BIT0;          // turn on to start
    // LED2
    P6DIR |= BIT6;          // Config as Output
    P6OUT |= BIT6;          // turn on to start

    // Port 3
    P3DIR |= 0xFF;
    P3OUT &= 0;

    // Timer B0
    // Math: 1s = (1*10^-6)(D1)(D2)(25k)    D1 = 5, D2 = 8
    TB0CTL |= TBCLR;        // Clear timer and dividers
    TB0CTL |= TBSSEL__SMCLK;  // Source = SMCLK
    TB0CTL |= MC__UP;       // Mode UP
    TB0CTL |= ID__8;         // divide by 8 
    TB0EX0 |= TBIDEX__5;    // divide by 5 (100)

    TB0CCR0 = 25000;

    TB0CCTL0 &= ~CCIFG;     // Clear CCR0
    TB0CCTL0 |= CCIE;       // Enable IRQ
    // Timer B1
    // Math: 1s = (1*10^-6)(D1)(D2)(25k)    D1 = 5, D2 = 8
    TB1CTL |= TBCLR;        // Clear timer and dividers
    TB1CTL |= TBSSEL__SMCLK;  // Source = SMCLK
    TB1CTL |= MC__UP;       // Mode UP
    TB1CTL |= ID__8;         // divide by 8 
    TB1EX0 |= TBIDEX__5;    // divide by 5 (100)

    base_transition_period = 25000;
    TB1CCR0 = base_transition_period;

    TB1CCTL0 &= ~CCIFG;     // Clear CCR0
    TB1CCTL0 |= CCIE;       // Enable IRQ
    
//------------- END PORT SETUP -------------------

    __enable_interrupt();   // enable maskable IRQs
    PM5CTL0 &= ~LOCKLPM5;   // turn on GPIO


    init_LED(status_led);


    // init_unused();
}



int main(void)
{
    Keypad keypad = {
        .lock_state = LOCKED,                           // locked is 1
        .row_pins = {BIT3, BIT2, BIT1, BIT0},      // order is 5, 6, 7, 8
        .col_pins = {BIT4, BIT5, BIT2, BIT0},    // order is 1, 2, 3, 4
        .passkey = {'1','1','1','1'},
    };

    char pk_attempt[4] = {'x','x','x','x'};
    cur_char = 'Z';
    int ret = FAILURE;
    int count = 0;


    init_keypad(&keypad);
    init();
    
    // set_LED(&status_led, MIDUNLOCK);   
    // set_LED(&status_led, UNLOCKED); 
    // set_LED(&status_led, PATTERN0); 
    // set_LED(&status_led, PATTERN1); 
    // set_LED(&status_led, PATTERN2); 
    // set_LED(&status_led, PATTERN3); 
    // set_LED(&status_led, PATTERN4); 
    // set_LED(&status_led, PATTERN5); 
    // set_LED(&status_led, PATTERN6); 
    // set_LED(&status_led, PATTERN7);

    while(true)
    {
        ret = scan_keypad(&keypad, &cur_char);
        __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
        // consider moving the following code to keypad.c
        if (ret == SUCCESS)
        {
            if (cur_char == 'D')
            {
                P3OUT &= 0;
                set_lock(&keypad, LOCKED);
                set_LED(&status_led, LEDLOCKED);
                count = 0;
            } 
            else if (keypad.lock_state == LOCKED)  // if we're locked and trying to unlock
            {
                P3OUT &= 0;
                pk_attempt[count] = cur_char;
                count++;
                if (count == 4)
                {
                    if(check_status(&keypad, pk_attempt) == SUCCESS)
                    {
                        set_lock(&keypad, UNLOCKED);
                        set_LED(&status_led, LEDUNLOCKED);
                    }
                    count = 0;
                }
                else if (count > 0)
                {
                    set_LED(&status_led, MIDUNLOCK);
                }
            }
            else            // unlocked so do pattern stuff. use p3.0-3.7, and 6.4, 6.5 for MSB
            {
                switch(cur_char)
                {
                    case 'A': // can't do a period of 0
                        if(base_transition_period > 6250)
                        {
                            base_transition_period -= 6250;
                            TB1CTL &= ~MC__UP;
                            TB1CCR0 = base_transition_period;
                            TB1CTL |= MC__UP;  // start timer again in up mode
                        }
                        break;
                    case 'B': // hard ceiling at 60k
                        if(base_transition_period < 60000)
                        {
                            base_transition_period += 6250;
                            TB1CTL &= ~MC__UP;
                            TB1CCR0 = base_transition_period;
                            TB1CTL |= MC__UP;  // start timer again in up mode
                        }
                        break;
                    case '0': // put pattern 0 on ports
                        if(status_led.current_state != PATTERN0)
                        {
                            set_LED(&status_led, PATTERN0);
                        }
                        else        // pattern selected twice in a row, reset
                        {
                            patterns[0] = default_patterns[0];
                        }
                        break;
                    case '1': // put pattern 1 on ports
                        if(status_led.current_state != PATTERN1)
                        {
                            set_LED(&status_led, PATTERN1);
                        }
                        else
                        {
                            patterns[1] = default_patterns[1];
                        }
                        break;

                    case '2': // put pattern 2 on ports
                        if(status_led.current_state != PATTERN2)
                        {
                            set_LED(&status_led, PATTERN2);
                        }
                        else
                        {
                            patterns[2] = default_patterns[2];
                        }
                        break;

                    case '3': // put pattern 3 on ports
                        if(status_led.current_state != PATTERN3)
                        {
                            set_LED(&status_led, PATTERN3);
                        }
                        else
                        {
                            patterns[3] = default_patterns[3];
                        }
                        break;
                    case '4': // put pattern 4 on ports
                        if(status_led.current_state != PATTERN4)
                        {
                            set_LED(&status_led, PATTERN4);
                        }
                        else
                        {
                            patterns[4] = default_patterns[4];
                        }
                        break;
                    case '5': // put pattern 5 on ports
                        if(status_led.current_state != PATTERN5)
                        {
                            set_LED(&status_led, PATTERN5);
                        }
                        else
                        {
                            patterns[5] = default_patterns[5];
                        }
                        break;
                    case '6': // put pattern 6 on ports
                        if(status_led.current_state != PATTERN6)
                        {
                            set_LED(&status_led, PATTERN6);
                        }
                        else
                        {
                            patterns[6] = default_patterns[6];
                        }
                        break;
                    case '7': // put pattern 7 on ports
                        if(status_led.current_state != PATTERN7)
                        {
                            set_LED(&status_led, PATTERN7);
                        }
                        else
                        {
                            patterns[7] = default_patterns[7];
                        }
                        break;

   
                    default:
                        break;
                }
            }
        }
        __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
    }

    return(0);
}



//-- Interrupt Service Routines -----------------------


// Heartbeat LED
#pragma vector = TIMER0_B0_VECTOR
__interrupt void heartbeatLED(void)
{
    P1OUT ^= BIT0;          // LED1 xOR
    TB1CCTL0 &= ~CCIFG;     // clear flag
}
// ----- end heartbeatLED-----

// UpdatePattern
#pragma vector = TIMER1_B0_VECTOR
__interrupt void updatePattern(void)
{
    P6OUT ^= BIT6;
    switch(status_led.current_state)
    {
        case PATTERN0:
            P3OUT = patterns[0];
            break;
        case PATTERN1: 
            patterns[1] ^= 0xFF;    // flip every bit
            P3OUT = patterns[1];
            break;
        case PATTERN2: // put pattern 0 on ports
            patterns[1] ^= 0xFF;    // flip every bit
            P3OUT = patterns[1];
            break;
       case PATTERN3:
            P3OUT = patterns[0];
            break;
        case PATTERN4: 
            patterns[1] ^= 0xFF;    // flip every bit
            P3OUT = patterns[1];
            break;
        case PATTERN5: // put pattern 0 on ports
            patterns[1] ^= 0xFF;    // flip every bit
            P3OUT = patterns[1];
            break;
        case PATTERN6: 
            patterns[1] ^= 0xFF;    // flip every bit
            P3OUT = patterns[1];
            break;
        case PATTERN7: // put pattern 0 on ports
            patterns[1] ^= 0xFF;    // flip every bit
            P3OUT = patterns[1];
            break;
        default:
            P3OUT = 0x00;
            break;
    }
    TB1CCTL1 &= ~CCIFG;     // clear flag
}
// ----- end updatePattern-----
