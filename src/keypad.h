#ifndef  KEYPAD_H
#define KEYPAD_H
    #include "msp430fr2355.h"
#include <stdio.h>
    #include </msp430.h>

    #ifndef DMN
    #define DMN 4
    #endif


    typedef struct {
        int lockState;
        int rowPins[4] = {BIT3, BIT2, BIT1, BIT0};      // order is 5, 6, 7, 8
        int colPins[4] = {BIT4, BIT5, BIT2, BIT0};      // order is 1, 2, 3, 4
        int password;
    } Keypad;

    extern char keyChar [DMN][DMN];

    void init_keypad(Keypad *keypad);
    void setLock(Keypad *keypad);
    void scanKeypad(Keypad *keypad);

#endif
