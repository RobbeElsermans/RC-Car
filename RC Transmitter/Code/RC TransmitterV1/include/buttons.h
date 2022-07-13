#ifndef BUTTONS_H
#define BUTTONS_H

// #include "pinout.h"
// #include "Arduino.h"

typedef enum{
    NONEBUTTON,
    BUTTON1,
    BUTTON2,
    BUTTON3,
    BUTTON4,
} buttonState;

void initButtons();
int readRawButtons();
buttonState readButtons();

#endif