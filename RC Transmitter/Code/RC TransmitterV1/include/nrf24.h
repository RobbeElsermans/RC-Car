#ifndef NRF24_H
#define NRF24_H
#include "main.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

const uint64_t addressSend =    0xE8E8F0F0E1LL;
const uint64_t addressReceive = 0xE877F0F0E1LL;

void initRadio();

void radioSetSend();
bool writeRadio(dataSend_t data);
void radioSetReceive();
void readRadio(dataReceive_t *data);

#endif