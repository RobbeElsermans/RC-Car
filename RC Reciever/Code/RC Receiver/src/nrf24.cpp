#include "pinout.h"
#include "nrf24.h"
#include "Arduino.h"

static RF24 radioObject(CE, CSN);

void radioSetSend(){
    radioObject.begin();
    radioObject.openWritingPipe(addressReceive);
    radioObject.setPALevel(RF24_PA_MIN);
    radioObject.stopListening();
}
void writeRadio(dataReceive_t data){
    radioObject.write(&data, sizeof(dataReceive_t));
}
void radioSetReceive(){
    radioObject.begin();
    radioObject.openReadingPipe(0, addressSend);
    radioObject.setPALevel(RF24_PA_HIGH);
    radioObject.startListening();
}
void readRadio(dataSend_t *data){
    if (radioObject.available()) {
        radioObject.read(data, sizeof(*data));
  }
}