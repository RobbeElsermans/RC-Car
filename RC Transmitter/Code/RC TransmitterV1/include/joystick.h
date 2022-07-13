#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "stdint.h"

void initJoy();

int readValX();
int readValY();

int readRawValX();
int readRawValY();

#endif
