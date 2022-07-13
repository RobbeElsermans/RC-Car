#include "batteryVal.h"
#include "Arduino.h"
#include "pinout.h"

const float divider = 0.25;

//Bij 3.3V
//const float vPerDiv = 0.003222656;

//Bij 5V
const float vPerDiv = 0.004887586;

void initBatteryVal(){
    pinMode(BATVAL, INPUT);
}

int getRawBatteryVal()
{
    return analogRead(BATVAL);
}

float getBatteryVal(){
    float raw = getRawBatteryVal();
    float temp = raw*vPerDiv;
    return temp/divider;
}