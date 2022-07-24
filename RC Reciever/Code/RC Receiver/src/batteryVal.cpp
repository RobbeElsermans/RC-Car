#include "batteryVal.h"
#include "Arduino.h"
#include "pinout.h"

const float divider = 0.235;

#define MEAN_COUNT 30
static float meanVal[MEAN_COUNT];
static uint8_t meanValCounter;

// Bij 3.3V
const float vPerDiv = 0.003222656;

// Bij 5V
// const float vPerDiv = 0.004887586;

float calcBattery(float rawData);

void initBatteryVal()
{
    pinMode(BATVAL, INPUT);
}

int getRawBatteryVal()
{
    return analogRead(BATVAL);
}

float getBatteryVal()
{
    if (meanVal[MEAN_COUNT - 1] == 0){   //Initieel de lijst opvullen
        uint8_t tempCounter = 0;
        while (meanVal[MEAN_COUNT - 1] == 0)
        {
            meanVal[tempCounter] = calcBattery(getRawBatteryVal());
            tempCounter++;
        }
    }
    else{
        if(meanValCounter >= MEAN_COUNT)
            meanValCounter = 0;

        meanVal[meanValCounter] = calcBattery(getRawBatteryVal());
        
        meanValCounter++;
    }
    
    //Calculate mean battery value
    float tempBatteryValue = 0;
    for (uint8_t i = 0; i < MEAN_COUNT; i++)
    {
        tempBatteryValue += meanVal[i];
    }
    tempBatteryValue /= (double)MEAN_COUNT;
    
    return tempBatteryValue;
    // float raw = getRawBatteryVal();
    // float temp = raw * vPerDiv;
    // return temp / divider;
}

float calcBattery(float rawData)
{
    float temp = rawData * vPerDiv;
    return temp / divider;
}