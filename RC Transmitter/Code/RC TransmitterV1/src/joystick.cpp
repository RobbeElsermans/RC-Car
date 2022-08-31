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
    int temp2 = map(temp, 0, 1023, yawMinOffset + yawCenterOffset, yawMaxOffset + yawCenterOffset);
    // if (temp2 > 0) // Positief
    // {
    //     if (temp2 <= yawMinOffset + yawCenterOffset || temp2 <= yawMinOffset)
    //         temp2 = yawMinOffset;

    //     if (temp2 >= yawMaxOffset - yawCenterOffset || temp2 >= yawMaxOffset)
    //         temp2 = yawMaxOffset;
    // }
    // if (temp2 < 0) // Negatief
    // {
    //     if (temp2 <= yawMinOffset - yawCenterOffset || temp2 <= yawMinOffset)
    //         temp2 = yawMinOffset;

    //     if (temp2 >= yawMaxOffset + yawCenterOffset || temp2 >= yawMaxOffset)
    //         temp2 = yawMaxOffset;
    // }

    return temp2;
}
int readValY()
{
    int temp = readRawValY();
    return map(temp, 0, 1023, throttleMinOffset, throttleMaxOffset);
}

int readRawValX()
{
    return analogRead(JOY_Y1);
}
int readRawValY()
{
    return analogRead(JOY_X1);
}