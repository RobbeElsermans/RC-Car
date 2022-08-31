#ifndef BUTTONS_H
#define BUTTONS_H

// #include "pinout.h"
// #include "Arduino.h"

typedef enum{
    NONEBUTTON,
    BUTTON1,    //BOVEN
    BUTTON2,    //RECHTS
    BUTTON3,    //ONDER
    BUTTON4,    //LINKS
} buttonState;

void initButtons();
int readRawButtons();
buttonState readButtons();

#endif