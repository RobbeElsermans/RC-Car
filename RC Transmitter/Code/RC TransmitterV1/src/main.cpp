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

void displaySignal();
void displayBatteryVoltage();
void displayMenu();
void displayCursor();
void checkCursorState();

uint8_t displaysCursorState = 0;

typedef enum
{
  MAIN,
  CORRECTIONS,
  DEBUG,
} displayMenuState;

displayMenuState displayState = MAIN;

#define MAX_MAIN_OBJECTS 3
buttonState prevButtonstate = NONEBUTTON;

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

  displaySignal();
  displayBatteryVoltage();
  displayMenu();
  displayCursor();
  checkCursorState();
}

void loop()
{
  // Serial.println(getThrottle());

  collectData();

  send();

  delay(1);

  if (millis() - displayTime > 200 && counterTotal == 100)
  {
    displaySignal();
    displayBatteryVoltage();
    displayMenu();
    displayCursor();
    checkCursorState();
    displayTime = millis();

    Serial.println(displayState);
  }

  if (counterTotal == 100)
  {
    counter = 0;
    counterTotal = 0;
  }
}

void send()
{
  dataOut.AUX4 = 1;
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

void displaySignal()
{
  u8x8.setFont(u8x8_font_open_iconic_embedded_1x1); // choose a suitable font
  u8x8.drawGlyph(0, 0, 80);                         // Draw glyph

  u8x8.setFont(u8x8_font_chroma48medium8_r); // choose a suitable font
  u8x8.drawString(3, 0, " ");
  u8x8.setCursor(1, 0);
  u8x8.print(counter);
  u8x8.drawString(4, 0, "%");
}
void displayBatteryVoltage()
{
  u8x8.setFont(u8x8_font_open_iconic_embedded_1x1); // choose a suitable font
  u8x8.drawGlyph(11, 0, 73);                        // Draw glyph

  u8x8.setFont(u8x8_font_chroma48medium8_r); // choose a suitable font
  u8x8.setCursor(12, 0);
  u8x8.print(getBatteryVal());
}

void displayMenu()
{
  switch (displayState)
  {
  case MAIN:
    u8x8.setFont(u8x8_font_open_iconic_embedded_1x1); // choose a suitable font
    u8x8.drawGlyph(2, 1, 68);                         // Draw glyph
    u8x8.drawGlyph(2, 2, 66);                         // Draw glyph
    u8x8.drawGlyph(2, 3, 72);                         // Draw glyph
    break;
  case CORRECTIONS:

    break;
  case DEBUG:
    u8x8.setFont(u8x8_font_open_iconic_arrow_1x1); // choose a suitable font
    u8x8.drawGlyph(0, 1, 67);                      // Draw glyph
    u8x8.drawGlyph(1, 1, 64);                      // Draw glyph
    u8x8.drawGlyph(0, 2, 65);                      // Draw glyph
    u8x8.drawGlyph(1, 2, 66);                      // Draw glyph

    u8x8.setFont(u8x8_font_chroma48medium8_r); // choose a suitable font
    u8x8.setCursor(2, 1);
    u8x8.print(dataOut.throttle);
    u8x8.setCursor(2, 2);
    u8x8.print(dataOut.yaw);

    u8x8.drawString(7, 1, "1");
    u8x8.setCursor(9, 1);
    u8x8.print(dataOut.AUX1);

    u8x8.drawString(7, 2, "2");
    u8x8.setCursor(9, 2);
    u8x8.print(dataOut.AUX2);

    u8x8.drawString(11, 1, "3");
    u8x8.setCursor(13, 1);
    u8x8.print(dataOut.AUX3);
    u8x8.drawString(11, 2, "4");
    u8x8.setCursor(13, 2);
    u8x8.print(dataOut.AUX4);

    break;

  default:
    break;
  }
}

void displayCursor()
{
  u8x8.setFont(u8x8_font_open_iconic_arrow_1x1); // choose a suitable font

  switch (displayState)
  {
  case MAIN:
    for (uint8_t i = 0; i < MAX_MAIN_OBJECTS; i++)
    {
      if (i != displaysCursorState)
        u8x8.drawString(0, 1 + i, " "); // Draw glyph
      else
        u8x8.drawGlyph(0, 1 + i, 78); // Draw glyph
    }
    break;
  case CORRECTIONS:

    break;
  case DEBUG:
    break;

  default:
    break;
  }
}

void checkCursorState()
{
  switch (displayState)
  {
  case MAIN:
    if (prevButtonstate != readButtons())
    {
      switch (readButtons())
      {
      case BUTTON1:
        displaysCursorState++;
        if (displaysCursorState >= MAX_MAIN_OBJECTS)
          displaysCursorState = 0;
        break;
      case BUTTON2:
        if (displaysCursorState == 0)
        {
          displayState = MAIN;
        }
        if (displaysCursorState == 1)
        {
          displayState = CORRECTIONS;
        }
        if (displaysCursorState == 2)
        {
          displayState = DEBUG;
        }
        u8x8.clearLine(1);
        u8x8.clearLine(2);
        u8x8.clearLine(3);
        break;
      case BUTTON3:
        displaysCursorState--;
        if (displaysCursorState == 255)
          displaysCursorState = MAX_MAIN_OBJECTS - 1;
        break;
      case BUTTON4:
        /* code */
        break;
      case NONEBUTTON:
        /* code */
        break;
      default:
        break;
      }
    }
    prevButtonstate = readButtons();
    break;
  case CORRECTIONS:
    if (prevButtonstate != readButtons())
    {
      switch (readButtons())
      {
      case BUTTON1:
        break;
      case BUTTON2:
        break;
      case BUTTON4:
        displayState = MAIN;
        break;
      case NONEBUTTON:
        /* code */
        break;
      default:
        break;
      }
    }
    prevButtonstate = readButtons();
    break;
  case DEBUG:
    /* code */
    break;

  default:
    break;
  }
}