#include "pinConfiguration.h"
#include "pinout.h"
#include "Arduino.h"


void pinSetup()
{
    pinMode(BATVAL, INPUT);
    pinMode(BUTTONS, INPUT);
    pinMode(NRF_INT, INPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
}