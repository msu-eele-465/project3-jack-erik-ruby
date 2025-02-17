#include "../src/keypad.h"
#include <msp430.h>

// reversed to match datasheet to match pin indices
char key_chars[4][4] = {
    {'D', '#', '0', '*'},  
    {'C', '9', '8', '7'},  
    {'B', '6', '5', '4'},  
    {'A', '3', '2', '1'}  
};

void init_keypad(Keypad *keypad) {
    // set keypad row pins as input
    int i;
    for (i = 0; i < 4; i++){
        P6DIR &= ~(keypad->row_pins[i]);        // set pin to input
        P6REN |= keypad->row_pins[i];         // enable pull up/down resistor
        P6OUT |= keypad->row_pins[i];         // set pull up resistor
    }

    // set keypad col pins as output
    for (i = 0; i < 4; i++){
        P2DIR |= keypad->col_pins[i];         // set pin to output
        P2OUT |= keypad->col_pins[i];         // set high
    }

    // this functionality is already hardcoded by the struct init
    // set passkey, 
    // for (i = 0; i < 4; i++){
    //     keypad->password[i] = passkey[i];
    // }

    // lock keypad
    // keypad->lockState = LOCKED;
}
void set_lock(Keypad *keypad, int lock) {
    if (lock != LOCKED)
        keypad->lock_state = UNLOCKED;
    else
        keypad->lock_state = LOCKED;
}
int scan_keypad(Keypad *keypad, char *key_press) {
    // for each col, check if there is a LOW row
    int col, row;

    for(col = 0; col < 4; col++){
        // clear, set col LOW
        P2OUT &= ~keypad->col_pins[col];
        __delay_cycles(1000);

        for(row = 0; row < 4; row++){
            if (!(P6IN & keypad->row_pins[row])){
                printf("got input! switch from %c to %c\n", *key_press, key_chars[row][col]);
                *key_press = key_chars[row][col];
                // set col HIGH
                P2OUT |= keypad->col_pins[col];
                return SUCCESS;
            }
        }
        // set col HIGH
        P2OUT |= keypad->col_pins[col];
    }
    return FAILURE;
}

int compare_pw(char *passkey, char *guess){
    int current;
    for (current = 0; current < 4; current++){
        if (passkey[current] != guess[current]){
            int i;
            for (i = 0; i < 4; i++){
                guess[i] = 'x';
            }
            return FAILURE;
        }
            
    }
    return SUCCESS;
}