#include "pinout.h"
#include "nrf24.h"
#include "Arduino.h"

static RF24 radioObject(CE, CSN);

static bool isSetRecieve = true;

void radioSetSend(){
    if(isSetRecieve){
        radioObject.closeReadingPipe(addressSend);
        radioObject.stopListening();
    }

    radioObject.begin();
    radioObject.openWritingPipe(addressReceive);
    radioObject.setPALevel(RF24_PA_MAX);
    radioObject.stopListening();
    isSetRecieve = false;
}
bool writeRadio(dataReceive_t data){
    return radioObject.write(&data, sizeof(dataReceive_t));
}
void radioSetReceive(){
    radioObject.begin();
    radioObject.openReadingPipe(0, addressSend);
    radioObject.setPALevel(RF24_PA_HIGH);
    radioObject.startListening();
    isSetRecieve = true;
}
void readRadio(dataSend_t *data){
    if (radioObject.available()) {
        radioObject.read(data, sizeof(*data));
  }
}