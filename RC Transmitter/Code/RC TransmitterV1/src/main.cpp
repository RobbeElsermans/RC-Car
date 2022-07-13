/**
 * @file main.cpp
 * @author Robbe Elsermans (robels@outlook.com)
 * @brief Main loop
 * @version 0.1
 * @date 2022-06-10
 *
 * This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS and shared in the Public Domain
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include "nrf24.h"
#include "main.h"
//#include "U8g2lib.h"
#include <U8x8lib.h>
#include "pinout.h"
#include "data.h"
#include "string.h"

#include "throttle.h"
#include "joystick.h"
#include "buttons.h"
#include "batteryVal.h"

/**
 * @brief Used private functions
 *
 */

void pinSetup();
void send();
void collectData();
int getThrottleJoy();

// U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
U8X8_SH1106_128X64_NONAME_SW_I2C u8x8(SCL, SDA);

dataReceive_t dataIn;
dataSend_t dataOut;

uint8_t counterTotal = 0;
uint8_t counter = 0;

unsigned long displayTime = 0;

void setup()
{
  Wire.setClock(400000);
  pinSetup();

  radioSetSend();

  // u8g2.begin();
  u8x8.begin();
  u8x8.setBusClock(400000);

  // Init serial output (debugging)
  Serial.begin(115200);
  // Serial.print("Robbe is cool met zijn RC Transmitter");

  dataOut = {
      1500,
      1500,
      0,
      0,
      0,
      0};

  dataIn = {
      0,
      0,
      0,
      0};

  displayTime = millis();
}

void loop()
{
  // Serial.println(getThrottle());

  collectData();

  send();

  delay(1);

  if (millis() - displayTime > 500 && counterTotal == 100)
  {
    // u8g2.clearBuffer();                                // clear the internal memory open_iconic_embedded_1x
    // u8g2.setFont(u8g2_font_open_iconic_embedded_1x_t); // choose a suitable font
    // u8g2.setCursor(0, 8);
    // u8g2.write(80);                     // write something to the internal memory
    // u8g2.setFont(u8g2_font_courB08_tf); // choose a suitable font
    // u8g2.setCursor(10, 8);
    // u8g2.print(counter);
    // u8g2.setCursor(0, 16);
    // u8g2.print(getThrottle());
    // u8g2.sendBuffer(); // transfer internal memory to the display

    // u8x8.clearLine(0);                                // clear line x
    u8x8.setFont(u8x8_font_open_iconic_embedded_1x1); // choose a suitable font
    u8x8.drawGlyph(0, 0, 80);                         // Draw glyph
    u8x8.drawGlyph(11, 0, 73);
    u8x8.setFont(u8x8_font_chroma48medium8_r); // choose a suitable font
    u8x8.drawString(3, 0, " ");
    u8x8.setCursor(1, 0);
    u8x8.print(counter);
    u8x8.drawString(4, 0, "%");

    u8x8.setCursor(12, 0);
    u8x8.print(getBatteryVal());

    u8x8.setCursor(0, 1);
    u8x8.print(dataOut.yaw);
  
    displayTime = millis();
  }

  if (counterTotal == 100)
  {
    counter = 0;
    counterTotal = 0;
  }
}

void send()
{
  bool stat = writeRadio(dataOut);
  if (stat)
  {
    counter++;
  }
  counterTotal++;
}

void collectData()
{
  if (getThrottle() != 1500)
  {
    dataOut.throttle = getThrottle();
  }
  else
  {
    dataOut.throttle = getThrottleJoy();
  }

  dataOut.yaw = getYawJoy();
}

void pinSetup()
{
  initButtons();
  initJoy();
  initRadio();
  initThrottle();

  pinMode(BATVAL, INPUT);
  pinMode(GYRO_INT, INPUT);
  pinMode(OLED_RESET, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}