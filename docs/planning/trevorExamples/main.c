#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

#include "dac.h"
#include "uart.h"
#include "parser.h"
#include "pmt_config.h"

// Set up DACs
// #pragma PERSISTENT stores these variables in FRAM so they persist across
// power cycles. This way the DAC voltages can be set once, rather than having
// to set them every time we power on the system.

__attribute__((persistent)) static struct dac_t dac2 =
{
    .sac_base_addr = SAC2_BASE,
    .port_base_addr = P3_BASE,
    .port_bit = BIT1,
    .data = 0
};

__attribute__((persistent)) static struct dac_t dac3 =
{
    .sac_base_addr = SAC3_BASE,
    .port_base_addr = P3_BASE,
    .port_bit = BIT5,
    .data = 0
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

    P6DIR |= BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    P6OUT &= ~(BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);
}


void init(void)
{
    // Disable watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Disable the GPIO power-on default high-impedance mode
    PM5CTL0 &= ~LOCKLPM5;

    // SET P1.0 direction as output
    P1DIR |= 0x01;

    init_dac(dac2);
    init_dac(dac3);

    init_uart();

    init_unused();
}


void main(void)
{
    char buf[BUF_SIZE];

    init();

    // Set the saved DAC voltages at startup
    set_dac_voltage(&dac2, dac2.data);
    set_dac_voltage(&dac3, dac3.data);

    P1OUT |= BIT1;

    while(true)
    {
        if(rxbuf_is_full())
        {
            get_rxbuf(buf);
            
            pmt_data_t parsed;
            
            if(parse_command(buf, &parsed))
            {
                if(parsed.pmt == SIGNAL)
                {
                    set_dac_voltage(&dac3, parsed.data);
                } else {
                    set_dac_voltage(&dac2, parsed.data);
                }
            } else {
                send_message("invalid input\r");
            }
            send_message(buf);
        }
    }
}
