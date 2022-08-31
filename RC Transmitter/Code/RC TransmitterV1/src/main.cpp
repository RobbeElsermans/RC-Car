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
#include "offset.h"
#include "EEPROM.h"

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
void writeEeprom();
void readEeprom();

uint8_t displaysCursorState = 0;

int throttleCenterOffset = 0;
int throttleMinOffset = 1000;
int throttleMaxOffset = 2000;
int yawCenterOffset = -50;
int yawMinOffset = 1200;
int yawMaxOffset = 1800;



typedef enum
{
  MAIN,
  CORRECTIONS,
  DEBUG,
} displayMenuState;

displayMenuState displayState = MAIN;

#define MAX_MAIN_OBJECTS 3
#define MAX_CORRECTIONS_OBJECTS 3
buttonState prevButtonstate = NONEBUTTON;

// U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(4, SCL, SDA);

dataReceive_t dataIn;
dataSend_t dataOut;

uint8_t counterTotal = 0;
uint8_t counter = 0;

bool hasSendData = false;

unsigned long displayTime = 0;

bool ledFlipper = false;

void setup()
{
  readEeprom();
  Wire.setClock(400000);
  pinSetup();

  radioSetSend();

  // u8g2.begin();
  u8x8.begin();
  u8x8.setBusClock(400000);

  // Init serial output (debugging)
  Serial.begin(9600);
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
      0};

  displayTime = millis();

  displaySignal();
  // displayBatteryVoltage();
  displayMenu();
  displayCursor();
  checkCursorState();
  //writeEeprom();
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
    // displayBatteryVoltage();
    displayMenu();
    displayCursor();
    checkCursorState();
    displayTime = millis();

    // Serial.println(displayState);
    // Serial.println(analogRead(BUTTONS));
    // Serial.print(dataOut.yaw);
    // Serial.print("   ");
    // Serial.println(dataOut.throttle);

    digitalWrite(LED1, ledFlipper);
    digitalWrite(LED2, !ledFlipper);
    ledFlipper = !ledFlipper;
  }

  if (counterTotal == 100)
  {
    counter = 0;
    counterTotal = 0;
  }

  // Serial.println(dataIn.batteryVal);
  //  Om de 3 seconden afgaan
  if (((millis() / 1000) % 3) == 0 && !hasSendData)
  {
    // Serial.print("2 seconden");
    // Serial.print("    ");
    // Serial.println(millis()/1000);

    // Sturen naar ontvanger dat we data willen binnen krijgen
    dataOut.AUX2 = 1;
    send();
    // Radio op luisteren zetten
    radioSetReceive();

    delay(5);
    readRadio(&dataIn);

    // do
    // {
    //   Serial.println("readData");
    //   readRadio(&dataIn);
    // } while (dataIn.batteryVal == 0);

    delay(2);

    radioSetSend();
    dataOut.AUX2 = 0;
    send();
    hasSendData = true;
  }
  else if (((millis() / 1000) % 3) != 0)
  {
    hasSendData = false;
  }
}

void send()
{
  dataOut.AUX4 = 1;
  bool stat = writeRadio(dataOut);
  // bool stat = true;
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
    //dataOut.throttle = getThrottle();
    dataOut.throttle = getThrottleJoy();
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
  u8x8.drawGlyph(10, 0, 73);                        // Draw glyph

  u8x8.setFont(u8x8_font_chroma48medium8_r); // choose a suitable font
  u8x8.setCursor(11, 0);
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

    u8x8.setFont(u8x8_font_open_iconic_embedded_2x2); // choose a suitable font
    u8x8.drawGlyph(5, 2, 73);                         // Draw glyph

    u8x8.setFont(u8x8_font_7x14B_1x2_r); // choose a suitable font
    u8x8.setCursor(7, 2);
    u8x8.print(dataIn.batteryVal / 100.0);
    break;
  case CORRECTIONS:
    u8x8.setFont(u8x8_font_chroma48medium8_r); // choose a suitable font
    u8x8.setCursor(2, 1);
    u8x8.print("steer ");
    u8x8.print(yawCenterOffset);
    u8x8.print(" ");
    //Value of steer Offset
    u8x8.setCursor(2, 2);
    u8x8.print("speed F ");
    u8x8.print(throttleMaxOffset);
    u8x8.print(" ");
    //Value of speed Forward Max
    u8x8.setCursor(2, 3);
    u8x8.print("speed R ");
    u8x8.print(throttleMinOffset);
    u8x8.print(" ");
    //Value of speed Reverse Max

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

    u8x8.setFont(u8x8_font_open_iconic_embedded_1x1); // choose a suitable font
    u8x8.drawGlyph(0, 3, 73);                         // Draw glyph

    u8x8.setFont(u8x8_font_chroma48medium8_r); // choose a suitable font
    u8x8.setCursor(2, 3);
    u8x8.print(dataIn.batteryVal / 100.0);

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
    for (uint8_t i = 0; i < MAX_MAIN_OBJECTS; i++)
    {
      if (i != displaysCursorState)
        u8x8.drawString(0, 1 + i, " "); // Draw glyph
      else
        u8x8.drawGlyph(0, 1 + i, 78); // Draw glyph
    }
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
      case BUTTON3:
        displaysCursorState++;
        if (displaysCursorState >= MAX_MAIN_OBJECTS)
          displaysCursorState = 0;
        break;
      case BUTTON2:
        if (displaysCursorState == 0)
        {
          displayState = MAIN;
          displaysCursorState = 0;
        }
        if (displaysCursorState == 1)
        {
          displayState = CORRECTIONS;
          displaysCursorState = 0;
        }
        if (displaysCursorState == 2)
        {
          displayState = DEBUG;
          displaysCursorState = 0;
        }
        u8x8.clearLine(1);
        u8x8.clearLine(2);
        u8x8.clearLine(3);
        break;
      case BUTTON1:
        displaysCursorState--;
        if (displaysCursorState == 255)
          displaysCursorState = MAX_MAIN_OBJECTS - 1;
        break;
      case BUTTON4:

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
  case CORRECTIONS: //kan enkel naar onder gaan door op de rechtste knop te drukken. De value kunnen we aanpassen door boven en onder knop te gebruiken
    if (prevButtonstate != readButtons())
    {
      switch (readButtons())
      {
      case BUTTON1: // BOVEN
        //De waarde laten stijgen
        if(displaysCursorState == 0){
          //Steering
          yawCenterOffset++;
        }
        if(displaysCursorState == 1){
          //FMaxThrottle
          throttleMaxOffset+=5;
        }
        if(displaysCursorState == 2){
          //RMaxThrottle
          throttleMinOffset+=5;
        }
        break;
      case BUTTON2: // RECHTS
      displaysCursorState++;
        if (displaysCursorState >= MAX_CORRECTIONS_OBJECTS)
          displaysCursorState = 0;
        break;
      case BUTTON3: // ONDER
        //De waarde laten dalen
        if(displaysCursorState == 0){
          //Steering
          yawCenterOffset--;
        }
        if(displaysCursorState == 1){
          //FMaxThrottle
          throttleMaxOffset-=5;
        }
        if(displaysCursorState == 2){
          //RMaxThrottle
          throttleMinOffset-=5;
        }
        break;
      case BUTTON4: // LINKS
        u8x8.clearLine(1);
        u8x8.clearLine(2);
        u8x8.clearLine(3);
        writeEeprom();
        displayState = MAIN;
        displaysCursorState = 0;
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
    if (prevButtonstate != readButtons())
    {
      switch (readButtons())
      {
      case BUTTON1:
        break;
      case BUTTON2:
        break;
      case BUTTON4:
        u8x8.clearLine(1);
        u8x8.clearLine(2);
        u8x8.clearLine(3);
        displayState = MAIN;
        displaysCursorState = 0;
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

  default:
    break;
  }
}

void writeEeprom(){
  eeprom_update_float((float *)0, yawCenterOffset);
  eeprom_update_float((float *)4, throttleMinOffset);
  eeprom_update_float((float *)8, throttleMaxOffset);
}
void readEeprom(){
  yawCenterOffset = eeprom_read_float((float *)0);
  throttleMinOffset = eeprom_read_float((float *)4);
  throttleMaxOffset = eeprom_read_float((float *)8);
}