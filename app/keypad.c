#include <msp430.h>
#include "../src/keypad.h"
#include "msp430fr2355.h"

char keyChars[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void init_keypad(Keypad *keypad) {
    // set keypad row GPIO pins
    for (int i = 0; i < 4; i++){
        P6DIR &= ~(1 << keypad->rowPins[i]);        // set pin to input
        P6REN |= (1 << keypad->rowPins[i]);         // enable pull up/down resistor
        P6OUT |= (1 << keypad->rowPins[i]);
        
    }

}
void setLock(Keypad *keypad) {

}
void scanKeypad(Keypad *keypad) {

}
