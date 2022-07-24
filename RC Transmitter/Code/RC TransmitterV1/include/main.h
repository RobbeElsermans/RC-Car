#ifndef MAIN_H
#define MAIN_H

#include "stdint.h"
#include "stdbool.h"

// Data structure van TRANSMITTER --> RECEIVER
typedef struct
{
  int throttle;
  int yaw;
  bool AUX1;
  bool AUX2;
  uint8_t AUX3;
  bool AUX4;
} dataSend_t;

// Data structure van RECEIVER --> TRANSMITTER
typedef struct
{
  bool batteryLow;
  int batteryVal;
} dataReceive_t;

#endif