/*
 * Erik Callery, EELE371, Lab 13.1
 * Last modified: March 4 2024 EC
 */
#include <msp430.h> 

int counter = 0;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

//------------- Setup Ports --------------------
    //LEDs
    P1DIR |= BIT0;          // Set P1.0 direction to out
    P1OUT &= ~BIT0;         // Clear P1.0 to start
    P6DIR |= BIT6;
    P6OUT &= ~BIT6;

    // Switches
    P4DIR &= ~BIT1;         // clear 4.1 direction = input
    P4REN |= BIT1;          // enable pull-up/down resistor
    P4OUT |= BIT1;          // make resistor a pull-up
    P2DIR &= ~BIT3;
    P2REN |= BIT3;          // enable pull-up/down resistor
    P2OUT |= BIT3;          // make resistor a pull-up

//    P4IFG &= ~BIT1;         // Clear flag
//    P4IE |= BIT1;           // Enable S1 IRQ
//    P2IFG &= ~BIT3;         // Clear flag
//    P2IE |= BIT3;           // Enable S2 IRQ
//
//    __enable_interrupt();   // enable GIE
    PM5CTL0 &= ~LOCKLPM5;   // turn on GPIO
//------------- END PORT SETUP -------------------

    int i, SW1, SW2;

    while(1)
    {
        SW1 = P4IN;         // Read Port4, put in SW1
        SW1 &= BIT1;        // Clear bits in SW1 except BIT1
        SW2 = P2IN;
        SW2 &= BIT3;

        // Look at switches and increment if SW1, Dec if SW2
        if(SW1 == 0)
        {
            ++counter;
        }
        else if(SW2 == 0)
        {
            --counter;
        }
        // Limit scope of counter
        if(counter > 3)
        {
            counter = 3;
        }
        else if(counter < 0)
        {
            counter = 0;
        }

        switch(counter)
        {
        case 1:
            P1OUT &= ~BIT0;        // disable LED1
            P6OUT |= BIT6;        // enable LED2
            break;
        case 2:
            P1OUT |= BIT0;
            P6OUT &= ~BIT6;
            break;
        case 3:
            P1OUT |= BIT0;
            P6OUT |= BIT6;
            break;
        default:                   // if count is 0
            P1OUT &= ~BIT0;
            P6OUT &= ~BIT6;
            break;
        }

        for(i = 0; i < 30000; ++i)  // Delay loop
        {
        }
        for(i = 0; i < 30000; ++i)  // Delay loop
        {
        }

    }

    return 0;
}

//-- Interrupt Service Routines -----------------------
//#pragma vector = PORT4_VECTOR
//__interrupt void ISR_increment_counter(void)
//{
//    ++counter;
//    P4IFG &= ~BIT1;
//}
//
//#pragma vector = PORT2_VECTOR
//__interrupt void ISR_decrement_counter(void)
//{
//    --counter;
//    P2IFG &= ~BIT3;
//}


