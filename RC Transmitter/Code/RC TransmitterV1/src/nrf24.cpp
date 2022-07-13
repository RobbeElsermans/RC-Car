#include "pinout.h"
#include "nrf24.h"
#include "Arduino.h"

static RF24 radioObject(CE, CSN);

void initRadio(){
    pinMode(NRF_INT, INPUT);
}

void radioSetSend(){
    radioObject.begin();
    radioObject.setRetries(0,0);
    radioObject.openWritingPipe(addressSend);
    radioObject.setPALevel(RF24_PA_HIGH);
    radioObject.stopListening();
}
bool writeRadio(dataSend_t data){
    return radioObject.write(&data, sizeof(data));
}
void radioSetReceive(){
    radioObject.begin();
    radioObject.openReadingPipe(0, addressReceive);
    radioObject.setPALevel(RF24_PA_MIN);
    radioObject.startListening();
}
void readRadio(dataReceive_t *data){
    if (radioObject.available()) {
        radioObject.read(data, sizeof(*data));
  }
}