#include "data.h"
#include "Arduino.h"
#include "throttle.h"
#include "joystick.h"
#include "offset.h"

int offsetYaw = -35;

int getThrottle()
{
    throttleState thState = NONETHROTTLE;

    thState = readState();

    if (thState == FRONT)
    {
        return 2000;
    }

    if (thState == BACK)
    {
        return 1000;
    }

    if (thState == NONETHROTTLE)
    {
        return 1500;
    }

    if(thState == BRAKE){
        return 500;
    }
    return NONETHROTTLE;
}

int getThrottleJoy(){
    return readValY();
}

int getYawJoy(){
    return readValX();
}

int getYaw(){
    throttleState thState = NONETHROTTLE;

    thState = readState();

    if (thState == FRONT)
    {
        return 2000;
    }

    if (thState == BACK)
    {
        return 1000;
    }

    if (thState == NONETHROTTLE)
    {
        return 1500;
    }
    return NONETHROTTLE;
}