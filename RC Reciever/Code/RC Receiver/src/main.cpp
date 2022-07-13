#include <Arduino.h>
#include "pinout.h"
#include "nrf24.h"
#include "main.h"
#include "motorDriver.h"
#include "Servo.h"

Servo steer;

dataReceive_t dataOut;
dataSend_t dataIn;

motorDriver motor(EN, L_PWM, R_PWM);

void pinSetup();

void setup()
{
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
  readRadio(&dataIn);

  Serial.println(dataIn.throttle);

  // if(dataIn.throttle > 1500)
  //   digitalWrite(LED1, HIGH);
  // else
  //   digitalWrite(LED1, LOW);

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

void pinSetup()
{
  pinMode(BATVAL, INPUT);
  pinMode(BUTTONS, INPUT);
  pinMode(NRF_INT, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}