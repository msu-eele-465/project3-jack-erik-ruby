/*
 * Erik Callery, EELE371, Lab 13.2
 * Last modified: March 6 2024 EC
 */
#include <msp430.h> 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "led_status.h"


// #pragma PERSISTENT stores these variables in FRAM so they persist across
// power cycles. This way the DAC voltages can be set once, rather than having
// to set them every time we power on the system.
__attribute__((persistent)) static status_LED status_led =
{
    .led_port_base_addr = P6_BASE,
    .red_port_bit = BIT0,
    .green_port_bit = BIT1,
    .blue_port_bit = BIT2,
    .current_state = LOCKED
};


void init_unused(void)
{
    //this just goes through every pin that is not
    //directly necessary, and puts it to output, pulled low

    P1DIR |= BIT5 | BIT4 | BIT3 | BIT2 | BIT1;
    P1OUT &= ~(BIT5 | BIT4 | BIT3 | BIT2 | BIT1);

    P2DIR |= BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    P2OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

    P3DIR |= BIT7 | BIT6 | BIT4 | BIT3 | BIT2 |BIT0;
    P3OUT &= ~(BIT7 | BIT6 | BIT4 | BIT3 | BIT2 | BIT0);

    P4DIR |= BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    P4OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

    P5DIR |= BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    P5OUT &= ~(BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

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

    // Timer B1
    // Math: 1s = (1*10^-6)(D1)(D2)(50k)    D1 = 5, D2 = 4
    TB1CTL |= TBCLR;        // Clear timer and dividers
    TB1CTL |= TBSSEL__SMCLK;  // Source = SMCLK
    TB1CTL |= MC__UP;       // Mode UP
    TB1CTL |= ID__4;         // divide by 4 (10)
    TB1EX0 |= TBIDEX__5;    // divide by 5 (100)
    TB1CCR0 = 50000;


    // Timer B1 Compare
    TB1CCTL0 &= ~CCIFG;     // Clear CCR0
    TB1CCTL0 |= CCIE;       // Enable IRQ
//------------- END PORT SETUP -------------------

    __enable_interrupt();   // enable maskable IRQs
    PM5CTL0 &= ~LOCKLPM5;   // turn on GPIO


    init_LED(status_led);

    // init_unused();
}


void main(void)
{

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
    //     printf("Hello World!\n");
    }
    return(0);
}



//-- Interrupt Service Routines -----------------------


// Heartbeat LED
#pragma vector = TIMER1_B0_VECTOR
__interrupt void heartbeatLED(void)
{
    P1OUT ^= BIT0;          // LED1 xOR
    TB1CCTL0 &= ~CCIFG;     // clear flag
}
// ----- end heartbeatLED-----
