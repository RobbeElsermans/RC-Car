#include <Arduino.h>
#include "pinout.h"
#include "nrf24.h"
#include "main.h"
#include "motorDriver.h"
#include "Servo.h"
#include "EasyBuzzer.h"
#include "batteryVal.h"

Servo steer;

dataReceive_t dataOut;
dataSend_t dataIn;

motorDriver motor(EN, L_PWM, R_PWM);

unsigned long errorCounter = 0;

unsigned long globalCounter = 0;

bool hasSendData = false;

unsigned long beepTimer = 0;
#define beepTime 2000
bool batteryLow = false;
bool beepSet = false;

void pinSetup();
void batteryCheck();
void connectionCheck();
void disableVehicleIfNeeded();
void setActuator();
void sendBatteryVoltage();

void setup()
{

  dataIn = {
      1500,
      1500,
      0,
      0,
      0,
      0};

  steer.attach(STEER);
  steer.writeMicroseconds(1500);
  motor.disable();
  pinSetup();
  radioSetReceive();
  Serial.begin(115200);
}

void loop()
{
  delay(1);
  batteryCheck();
  connectionCheck();
  disableVehicleIfNeeded();
  setActuator();
  sendBatteryVoltage();
}

void pinSetup()
{
  pinMode(BATVAL, INPUT);
  pinMode(BUTTONS, INPUT);
  pinMode(NRF_INT, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void batteryCheck(){
  if (getBatteryVal() <= 11.4 && !batteryLow)
  {
    // Beep om de seconden
    batteryLow = true;
    beepTimer = millis();
    motor.disable();
  }
  else if (getBatteryVal() > 11.6)
  {
    batteryLow = false;
    motor.enable();
  }

  if (batteryLow && (millis() - beepTimer) > beepTime)
  {
    beepTimer = millis();
    if (beepSet)
    {
      // Beep
      beepSet = false;
    }
    else
    {
      // geen Beep
      beepSet = true;
    }
  }
}

void connectionCheck(){
  dataIn.AUX4 = 0;
  dataIn.AUX2 = 0;
  readRadio(&dataIn);
  Serial.println(errorCounter);
  if (dataIn.AUX4 == 0)
  {
    errorCounter++;
  }
  else if (dataIn.AUX4 == 1)
  {
    errorCounter = 0;
  }
}

void disableVehicleIfNeeded(){
  if (errorCounter >= 500)
  {
    motor.disable();
    steer.detach();
    dataIn.throttle = 1500;
  }
  else{
    if(!steer.attached())
      steer.attach(STEER);
  }
}

void setActuator(){
  if (dataIn.throttle < 1520 && dataIn.throttle > 1480)
  {
    motor.disable();
  }
  else
  {
    if (dataIn.throttle > 1520)
    {
      motor.enable();
      motor.forward(map(dataIn.throttle, 1520, 2000, 0, 255));
    }
    if (dataIn.throttle < 1480)
    {
      if (dataIn.throttle == 500)
      {
        motor.enable();
        motor.stop();
      }
      else
      {
        motor.enable();
        motor.backward(map(dataIn.throttle, 1480, 1000, 0, 255));
      }
    }
  }

  steer.write(map(dataIn.yaw, 1000, 2000, 0, 180));
}

void sendBatteryVoltage(){
  if(dataIn.AUX2 == 1)
 {
    Serial.println("aux2");
    dataOut.batteryVal = getBatteryVal()*100;
    dataOut.batteryLow = batteryLow;

    radioSetSend();
    for (uint8_t i = 0; i < 6; i++)
    {
      writeRadio(dataOut);
    }
    
    // bool temp = false;
    // do
    // {
    //   temp = writeRadio(dataOut);
    // } while (temp == false);

    radioSetReceive();
 }
}