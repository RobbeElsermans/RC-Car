#ifndef DATAS_H
#define DATAS_H
#include "stdint.h"
#include "stdbool.h"


/**
 * @brief Get the Throttle value by SW_FRONT & SW_BACK
 * 
 * @return int 
 */
int getThrottle();

/**
 * @brief Get the Throttle value by JOY_Y
 * 
 * @return int 
 */
int getThrottleJoy();

/**
 * @brief Get the Yaw value by SW_FRONT & SW_BACK
 * 
 * @return int 
 */
int getYaw();

/**
 * @brief Get the Yaw value by JOY_X
 * 
 * @return int 
 */
int getYawJoy();


bool getAux1();
bool getAux2();
uint8_t getAux3();
bool getAux4();

#endif