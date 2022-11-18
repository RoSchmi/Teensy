#include "KeyFrequTable.h"

KeyFrequTable::KeyFrequTable(){}

int KeyFrequTable::getFrequ(const int key)
{
    int f = 0;  
    int k = key | 0xF000;
    switch (k) {
        
    // Notes for keyboard
    case KEY_A : f = 262; break;
    case KEY_S : f = 277; break;
    case KEY_D : f = 294; break;
    case KEY_F: f = 311; break;
    case KEY_G : f = 330; break;
    case KEY_H : f = 349; break;
    case KEY_J : f = 370; break;
    case KEY_K: f = 415; break;
    case KEY_L : f = 440; break;
    case KEY_SEMICOLON : f = 466; break;
    case KEY_QUOTE: f = 494; break;

    // Notes for keypad
    case KEYPAD_7 : f = 262; break;
    case KEYPAD_8 : f = 277; break;
    case KEYPAD_9 : f = 294; break;
    case KEYPAD_4: f = 311; break;
    case KEYPAD_5 : f = 330; break;
    case KEYPAD_6 : f = 349; break;
    case KEYPAD_1 : f = 370; break;
    case KEYPAD_2: f = 415; break;
    case KEYPAD_3 : f = 440; break;
    case KEYPAD_0 : f = 466; break;
    case KEYPAD_PERIOD: f = 494; break;

    default    : f = 0; break;
    }
    return f;
}
