#include <msp430.h>

#include "ledStatus.h"

#include <stdio.h>

// Make sure DAC_VOLTAGE_REF_MV is 1500 mV, since we don't currently
// support any other internal voltage references

void init_dac(struct dac_t dac)
{
    // I used section 2.5 of the following app note to figure out
    // how to configure the "smart analog combo" to operate in DAC mode
    // https://www.ti.com/lit/an/slaa833a/slaa833a.pdf

    // Define the register base addresses for the particular SACx module
    // that the dac struct is set up for. The base_addr in the dac struct
    // defines the base address of the SAC module. The register offsets for
    // each module are the same, so we use the SAC0 offset defintions, which
    // are defined in msp420fr2355.h
    uint16_t *DAC_REG = (uint16_t *) (dac.sac_base_addr + OFS_SAC0DAC);
    uint16_t *OA_REG = (uint16_t *) (dac.sac_base_addr + OFS_SAC0OA);
    uint16_t *PGA_REG = (uint16_t *) (dac.sac_base_addr + OFS_SAC0PGA);

    // Define the base addresses for the port multiplexing registers, PxSEL1 and
    // PxSEL0. These offsets are the same for all ports, and are defined in
    // msp430fr2355.h. We use the port 1 offsets.
    uint16_t *PX_SEL0 = (uint16_t *) (dac.port_base_addr + OFS_P1SEL0); 
    uint16_t *PX_SEL1 = (uint16_t *) (dac.port_base_addr + OFS_P1SEL1); 

    // Enable the 1.5 V internal reference
    /* NOTE: the MSP430FR2355 internal reference defaults to 1.5 V, but
             but can be changed to 2.0 or 2.5V. */
    // This will be enabled each time a DAC is initialized, but that's okay
    // because the bit will always be a 1 after the first initialization.
    PMMCTL2 |= INTREFEN;

    // Set the DAC reference to the internal 1.5 V reference
    *DAC_REG |= DACSREF_1;

    // Multiplex the pin to select the DAC output
    // The PxSELx is set to 0b11 to enable the DAC output on that pin.
    // The bit # in PxSEL corresponds to the pin # on that port. 
    // Both SEL1[pin #] and SEL0[pin #] need to be set to a 1.
    *PX_SEL1 |= dac.port_bit;
    *PX_SEL0 |= dac.port_bit;

    // Enable the DACs
    *DAC_REG |= DACEN;

    // Select the DAC as the positive input source
    *OA_REG |= PSEL_1;

    // Select the PGA source for the negative input; the PGA will be configured for buffer mode
    *OA_REG |= NSEL_1;

    // Set the PGA to buffer mode
    *PGA_REG |= MSEL_1;

    // Enable the positive input and negative input muxes
    *OA_REG |= PMUXEN | NMUXEN;

    // Enable the op-amp outputs
    *OA_REG |= OAEN;

    // Enable smart analog combo (SAC) module
    *OA_REG |= SACEN;
}

// set_dac_voltage() will convert the voltage into the correct 12-bit number, 
// then write that to the register.
void set_LED(struct status_LED *sL, State s)
{
    sL->currentState = s;

   switch (s) 
   {
    case LOCKED:        // set pwm to rgb(196 62 29)
        
        break;

    case MIDUNLOCK:     // rgb(196 146 29)
        
        break;

    case UNLOCKED:      // rgb(29 162 196)
        
        break;

    case PATTERN0:
       
        break;

    case PATTERN1:
       
        break;

    case PATTERN2:
        
        break;

    case PATTERN3:
        
        break;

    case PATTERN4:
        
        break;

    case PATTERN5:
        
        break;

    case PATTERN6:
        
        break;

    case PATTERN7:
        
        break;

    default:
        
        break;
    }


    // TODO: document this equation
    static const float CONVERSION_FACTOR = ((float)((2 << (DAC_N_BITS - 1)) - 1)) / ((float)DAC_VOLTAGE_REF_MV);

    uint16_t dac_data = (uint16_t) ((float)voltage * CONVERSION_FACTOR);

    uint16_t *SACDAT = (uint16_t *) (dac->sac_base_addr + OFS_SAC0DAT);

    *SACDAT = dac_data;
}
