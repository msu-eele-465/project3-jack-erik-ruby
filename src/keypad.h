#ifndef  KEYPAD_H
#define KEYPAD_H
    #include <msp430.h>
    #include <stdio.h>

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
        int lock_state;
        int row_pins[4];      // order is 5, 6, 7, 8
        int col_pins[4];      // order is 1, 2, 3, 4
        char passkey[4];
    } Keypad;

    extern char key_chars [4][4];

    void init_keypad(Keypad *keypad);
    void set_lock(Keypad *keypad, int lock);
    int scan_keypad(Keypad *keypad, char *key_press);
    int compare_pw(char passkey[], char guess[]);
    int check_status(Keypad *keypad, char pk_attempt[]);
    int reset_pk(char passkey[]);

#endif
