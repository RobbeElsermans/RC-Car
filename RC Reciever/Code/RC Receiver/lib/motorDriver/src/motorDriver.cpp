#include "motorDriver.h"

motorDriver::motorDriver(uint8_t _EN, uint8_t _L_PWM, uint8_t _R_PWM){
_R_Pin = _R_PWM;
_L_Pin = _L_PWM;
_EN_Pin = _EN;

pinMode(_R_Pin, OUTPUT);
pinMode(_L_Pin, OUTPUT);
pinMode(_EN_Pin, OUTPUT);
}

void motorDriver::enable(){
    digitalWrite(_EN_Pin, HIGH);
}

void motorDriver::disable(){
    digitalWrite(_EN_Pin, LOW);
    digitalWrite(_L_Pin, LOW);
    digitalWrite(_R_Pin, LOW);
}

void motorDriver::forward(uint8_t pwm){
    analogWrite(_R_Pin, pwm);
    digitalWrite(_L_Pin, LOW);
}

void motorDriver::backward(uint8_t pwm){
    analogWrite(_L_Pin, pwm);
    digitalWrite(_R_Pin, LOW);
}

void motorDriver::stop(){
    digitalWrite(_L_Pin, LOW);
    digitalWrite(_R_Pin, LOW);
}