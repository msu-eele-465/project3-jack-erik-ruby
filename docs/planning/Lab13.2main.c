/*
 * Erik Callery, EELE371, Lab 13.2
 * Last modified: March 6 2024 EC
 */
#include <msp430.h> 


int deltaT = 263;       // delta-t = 1050*.25 = 250
int T = 1050;           // T = 1/1M * N. N = 1000 (Used 1050 for an actual T= 1ms)

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

//------------- Setup Ports --------------------
    // LED1
    P1DIR |= BIT0;          // Config as Output
    P1OUT |= BIT0;          // turn on to start for dimmer

    // Switches
    P4DIR &= ~BIT1;         // clear 4.1 direction = input
    P4REN |= BIT1;          // enable pull-up/down resistor
    P4OUT |= BIT1;          // make resistor a pull-up
    P2DIR &= ~BIT3;
    P2REN |= BIT3;          // enable pull-up/down resistor
    P2OUT |= BIT3;          // make resistor a pull-up

    P4IFG &= ~BIT1;         // Clear flag
    P4IE |= BIT1;           // Enable S1 IRQ
    P2IFG &= ~BIT3;         // Clear flag
    P2IE |= BIT3;           // Enable S2 IRQ

    // Timer B0
    TB0CTL |= TBCLR;        // Clear timer and dividers
    TB0CTL |= TBSSEL__SMCLK;  // Source = SMCLK
    TB0CTL |= MC__UP;       // Mode UP
    TB0CCR0 = T;
    TB0CCR1 = deltaT;

    // Timer Compares
    TB0CCTL0 &= ~CCIFG;     // Clear CCR0
    TB0CCTL0 |= CCIE;       // Enable IRQ
    TB0CCTL1 &= ~CCIFG;     // Clear CCR1
    TB0CCTL1 |= CCIE;       // Enable IRQ

    __enable_interrupt();   // enable maskable IRQs
    PM5CTL0 &= ~LOCKLPM5;   // turn on GPIO
//------------- END PORT SETUP -------------------

    while(1)              // loops forever
    {

    }


    return 0;
}

//-- Interrupt Service Routines -----------------------

// TB0PeriodReached
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TB0PeriodReached(void)
{
    P1OUT |= BIT0;          // LED1 on
    TB0CCTL0 &= ~CCIFG;     // clear flag
}
// ----- end periodReached-----

// pulseLengthReached
#pragma vector = TIMER0_B1_VECTOR
__interrupt void pulseLengthReached(void)
{
    P1OUT &= ~BIT0;          // LED1 off
    TB0CCTL1 &= ~CCIFG;     // clear flag
}
// ----- end pulseLengthReached-----

// increaseDutyCycle
#pragma vector = PORT4_VECTOR
__interrupt void increaseDutyCycle(void)
{
    deltaT += 26;           // 26 is close to 2.5 % of T
    if(deltaT > 525)        // 50 % of 1 ms (N = 1050)
    {
        deltaT = 525;
    }
    else
    {
        TB0CCR1 = deltaT;
    }
    P4IFG &= ~BIT1;
}
// ----- end increaseDutyCycle-----

// decreaseDutyCycle
#pragma vector = PORT2_VECTOR
__interrupt void decreaseDutyCycle(void)
{
    deltaT -= 26;
    if(deltaT < 105)    // 10 % of 1 ms (1050 = N0
    {
        deltaT = 105;
    }
    else
    {
        TB0CCR1 = deltaT;
    }
    P2IFG &= ~BIT3;
}
// ----- end decreaseDutyCycle-----


