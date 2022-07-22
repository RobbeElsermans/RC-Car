#include "throttle.h"
#include "Arduino.h"
#include "pinout.h"

void initThrottle()
{
    pinMode(SW_BACK, INPUT);
    pinMode(SW_FRONT, INPUT);
}

bool readRawFront()
{
    return digitalRead(SW_FRONT);
}
bool readRawBack()
{
    return digitalRead(SW_BACK);
}

throttleState readState()
{
    bool temp1 = readRawFront();
    bool temp2 = readRawBack();

    if (temp1 && !temp2)
    {
        return BACK;
    }

    if (temp2 && !temp1)
    {
        return FRONT;
    }

    if (!temp1 && !temp2)
    {
        return BRAKE;
    }

    if (temp1 && temp2)
    {
        return NONETHROTTLE;
    }

    return NONETHROTTLE;
}