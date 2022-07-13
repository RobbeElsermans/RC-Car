#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "Arduino.h"

class motorDriver{
public:
    motorDriver(uint8_t _EN, uint8_t _L_PWM, uint8_t _R_PWM);
    void enable();
    void disable();
    void forward(uint8_t pwm);
    void backward(uint8_t pwm);
    void stop();

private:
    uint8_t _R_Pin;
    uint8_t _L_Pin;
    uint8_t _EN_Pin;
};

#endif