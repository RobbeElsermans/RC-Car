#include "buttons.h"
#include "pinout.h"
#include "Arduino.h"

void initButtons()
{
    pinMode(BUTTONS, INPUT);
}
int readRawButtons()
{
    return analogRead(BUTTONS);
}
buttonState readButtons()
{
    int temp = readRawButtons();

    if(temp > 900 && temp < 950){
        return BUTTON1;
    }
    else if(temp > 800 && temp < 850){
        return BUTTON2;
    }
    else if(temp > 650 && temp < 720){
        return BUTTON3;
    }
    else if(temp > 490 && temp < 550){
        return BUTTON4;
    }
    else{
        return NONEBUTTON;
    }
    return NONEBUTTON;
}