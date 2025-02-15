#ifndef  KEYPAD_H
#define KEYPAD_H
    #include "msp430fr2355.h"
    #include <stdio.h>
    #include </msp430.h>

    #ifndef LOCKED
    #define LOCKED 1
    #endif
    #ifndef UNLOCKED
    #define UNLOCKED 0
    #endif
    #ifndef SUCCESS
    #define SUCCESS 1
    #endif
    #ifndef FAILURE
    #define FAILURE 0
    #endif

    typedef struct {
        int lockState;
        int rowPins[4];      // order is 5, 6, 7, 8
        int colPins[4];      // order is 1, 2, 3, 4
        char passkey[4];
    } Keypad;

    extern char keyChar [4][4];

    void init_keypad(Keypad *keypad);
    void setLock(Keypad *keypad, int lock);
    void scanKeypad(Keypad *keypad);

#endif
