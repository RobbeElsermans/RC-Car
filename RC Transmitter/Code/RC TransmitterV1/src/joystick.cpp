#include "joystick.h"
#include "Arduino.h"
#include "pinout.h"
#include "offset.h"

void initJoy()
{
    pinMode(JOY_BUTTON, INPUT);
    pinMode(JOY_X1, INPUT);
    pinMode(JOY_Y1, INPUT);
}

int readValX()
{
    int temp = readRawValX();
    return map(temp, 0, 1024, yawMinOffset-yawCenterOffset, yawMaxOffset-yawCenterOffset) + yawCenterOffset;
}
int readValY()
{
    int temp = readRawValY();
    return map(temp, 0, 1024, 1000, 2000);
}

int readRawValX()
{
    return analogRead(JOY_Y1);
}
int readRawValY()
{
    return analogRead(JOY_X1);
}