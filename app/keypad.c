#include <msp430.h>
#include "../src/keypad.h"
#include "msp430fr2355.h"

// reversed to match datasheet to match pin indices
char keyChars[4][4] = {
    {'D', '#', '0', '*'},  
    {'C', '9', '8', '7'},  
    {'B', '6', '5', '4'},  
    {'A', '3', '2', '1'}  
};

void init_keypad(Keypad *keypad) {
    // set keypad row pins as input
    int i;
    for (i = 0; i < 4; i++){
        P6DIR &= ~(keypad->rowPins[i]);        // set pin to input
        P6REN |= keypad->rowPins[i];         // enable pull up/down resistor
        P6OUT |= keypad->rowPins[i];         // set pull up resistor
    }

    // set keypad col pins as output
    for (i = 0; i < 4; i++){
        P2DIR |= keypad->colPins[i];         // set pin to output
        P2OUT |= keypad->colPins[i];         // set high
    }

    // set passkey
    for (i = 0; i < 4; i++){
        keypad->password[i] = passkey[i];
    }

    // lock keypad
    keypad->lockState = LOCKED;
}
void setLock(Keypad *keypad, int lock) {
    if (lock != LOCKED)
        keypad->lockState = UNLOCKED;
    else
        keypad->lockState = LOCKED;
}
int scanKeypad(Keypad *keypad, char *keyPress) {
    // for each col, check if there is a LOW row
    for(int col = 0; col < 4; col++){
        // clear, set col LOW
        P2OUT &= ~keypad->colPins[col];

        for(int row = 0; row < 4; row++){
            if (!(P2IN & keypad->rowPins[row])){
                keyPress = keyChars[row][col];

                // set col HIGH
                P2OUT |= keypad->colPins[col];
                return SUCCESS;
            }
        }
        // set col HIGH
        P2OUT |= keypad->colPins[col];
    }
    return FAILURE;
}
