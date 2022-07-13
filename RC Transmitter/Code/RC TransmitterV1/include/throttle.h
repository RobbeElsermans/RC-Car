#ifndef THROTTLE_H
#define THROTTLE_H

#include "stdbool.h"

typedef enum{
    NONETHROTTLE,
    BRAKE,
    FRONT,
    BACK
}throttleState;

void initThrottle();

bool readRawFront();
bool readRawBack();

throttleState readState();
#endif