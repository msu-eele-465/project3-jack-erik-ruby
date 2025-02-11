#include <msp430.h>
#include <stdbool.h>

/**
 Jack Tjaden EELE 371 Lab 13.2
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT0;              //LED1
    P1OUT |= BIT0;

    PM5CTL0 &= ~LOCKLPM5;       //GPIO

    P4DIR &= ~BIT1; //set P4.1 direction = in
    P4REN |= BIT1; //enable PU/PD Resistor
    P4OUT |= BIT1; //set PU resistor
    P4IES |= BIT1; //set IRQ H-L

    P2DIR &= ~BIT3; //set P2.3 direction = in
    P2REN |= BIT3; //enable PU/PD Resistor
    P2OUT |= BIT3; //set PU resistor
    P2IES |= BIT3; //set IRQ H-L

    P4IFG &= ~BIT1; //clear p4.1 IRQ flag
    P4IE |= BIT1; //enable P4.1 IRQ
    P2IFG &= ~BIT3; //clear p2.3 IRQ flag
    P2IE |= BIT3; //enable P2.3 IRQ

    TB0CTL |= TBCLR; //Timer B0
    TB0CTL |= TBSSEL__SMCLK;
    TB0CTL |= MC__UP;
    TB0CCR0 = 1000; //period
    TB0CCR1 = 250; // duty

    TB0CCTL0 &= ~CCIFG; //cmp IRQ for CCR0 and CCR1
    TB0CCTL0 |= CCIE;
    TB0CCTL1 &= ~CCIFG;
    TB0CCTL1 |= CCIE;
    __enable_interrupt();

    while(1){}


    return 0;
}

//------------Interrupt Service Routines--------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void)
{
     P1OUT |= BIT0;//1
     TB0CCTL0 &= ~CCIFG;        //Clear CCR0 flag

}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB0_CCR1(void)
{
     P1OUT &= ~BIT0;//0
     TB0CCTL1 &= ~CCIFG;        //Clear CCR1 Flag


}

#pragma vector = PORT2_VECTOR
__interrupt void ISR_TB0_PORT2(void){

    TB0CCR1 += 25; //increment by 25 %

    if (TB0CCR1 > 500){TB0CCR1 = 500;} // check to see if it reaches 500

    P2IFG &= ~BIT3;        //Clear
}

#pragma vector = PORT4_VECTOR
__interrupt void ISR_TB0_PORT4(void){


    TB0CCR1 -= 25; //decrement by 25%

    if (TB0CCR1 < 25){TB0CCR1 = 25;} // check to see if it reaches 25

    P4IFG &= ~BIT1;        //Clear
}
