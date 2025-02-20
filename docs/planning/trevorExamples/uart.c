#include <msp430.h>
#include <stdbool.h>
#include <string.h>

#include "uart.h"

static volatile rxbuf_t rxbuf = 
{
    .index = 0,
    .is_full = false,
};

// TODO: make it graceful with interrupts
void send_message(const char *buf)
{
    int i = 0;
    int j;
    while(buf[i] != '\0')
    {
        UCA0TXBUF = buf[i];      

        // delay loop to slow down UART transmission.
        // TODO: there must be a better way to do this and actually send the chars at the baud rate, but this is just temporary for now...
        for (j = 0; j < 1000; j++)
        {
            __asm__("NOP");
        }
        i++;
    }

    UCA0TXBUF = '\n';
}

void init_uart(void)
{
    // Put USCI_A0 into software reset
    // It needs to be in software reset for us to modify some register settings
    UCA0CTLW0 |= UCSWRST;

    // Use the SMCLK as the UART clock
    // SMCLK defaults to 1 MHz?
    UCA0CTLW0 |= UCSSEL__SMCLK;
    
    // Set baud rate to 115200 bps.
    // SMCLK is set to 1 MHz by default, so that's what I'm using as the UART clock
    // Calculations are from here: 
    // https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
    
    // Set the prescaler to 8
    UCA0BRW = 8;

    // Set the secondary modulation stage to 214
    UCA0MCTLW |= 0xd600;

    /*
     Multiplex the UART pins to P1.6 (UCA0RXD) and P1.7 (UCA0TXD)
     */
    // Mux RXD (P1.6)
    P1SEL1 &= ~BIT6;
    P1SEL0 |= BIT6;
    // Mux TXD (P1.7)
    P1SEL1 &= ~BIT7;
    P1SEL0 |= BIT7;

    // Take USCI_A0 out of software reset
    UCA0CTLW0 &= ~UCSWRST;

    // Enable the receive interrupt
    UCA0IE |= UCRXIE;
    __enable_interrupt();
}

bool rxbuf_is_full(void)
{
    return rxbuf.is_full;
}

int get_rxbuf(char *buf)
{
    if (rxbuf.is_full)
    {
        strncpy(buf, rxbuf.data, BUF_SIZE);
        
        // XXX: once I read the buffer, I want it so we can't keep reading the same buffer over and over again... I would like a better way to do this, though.
        rxbuf.is_full = false;
        return 0;
    }
    else
    {
        return -1;
    }
}

#pragma vector = EUSCI_A0_VECTOR
__interrupt void uart_rx_isr(void)
{
    if (rxbuf.is_full)
    {
        // We already received a full string according to our
        // protocol, so we'll restart at the beginning for the
        // new string.
        rxbuf.is_full = false;
        rxbuf.index = 0;
    }

    if (rxbuf.index < BUF_SIZE - 1)
    {
        // Add the new character to the end of the buffer
        rxbuf.data[rxbuf.index] = UCA0RXBUF;
        rxbuf.index += 1;

        // Carriage return indicates that the sender is done
        // with their string
        if (rxbuf.data[rxbuf.index - 1] == '\r')
        {
            // Null-terminate the string
            rxbuf.data[rxbuf.index] = '\0';
            
            // XXX: the buffer isn't necessarily full here;
            // in this case, this really means that we got
            // a carriage return --- that's all. However, a
            // carriage return indicates that the sender is done
            // sending, so we'll assume that they sent their
            // entire message even if it's smaller than our buffer size.
            rxbuf.is_full = true;

            // The current string is over, so we'll start at the beginning
            // of the buffer for the next character that's sent.
            rxbuf.index = 0;

            // TODO: remove LED toggle
            P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
        }
    }
    else
    {
        // The sender reached the end of buffer. End the buffer with a
        // null character so just so we don't have any weird buffer
        // overruns down the line.
        rxbuf.data[BUF_SIZE - 1] = '\0';

        rxbuf.is_full = true;
        
        // Restart at the beginning of the buffer next time.
        rxbuf.index = 0;
    }

    // Manually clear the interrupt flag. Reading UCAxRXBUF
    // automatically clears the interrupt flag, but we don't
    // bother reading the buffer when the sender reaches the end
    // of the buffer (since we always want the buffer to end with
    // a null character). If the interrupt flag doesn't get cleared,
    // the interrupt will fire again. We'll always clear the interrupt
    // flag here at the end even if it has already been cleared by
    // reading UCAxRXBUF.
    UCA0IFG &= ~UCRXIFG;
}
