#include "base.h"

// HardwareSerial PzemSerial(1);
int chipId = 0;

Main::Main(long baudRate) : _baudRate(baudRate){}

void Main::setup() {
    Serial.begin(_baudRate);
    this->rtcSetup();
    this->pzemSetup();
    this->gpsSetup();
}

String Main::chipID(){
    for (int i = 0; i < 17; i = i + 8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xFF) << i;
    }
    return String(chipId);
}

char* Main::encodePayload(char* inputString){
    int encodedLen = Base64.encodedLength(strlen(inputString));
    char encodedData[encodedLen + 1];
    
    Base64.encode(encodedData, inputString, strlen(inputString));
    encodedData[encodedLen] = '\0';

    return encodedData;
}

int Main::ceil(int value1, int value2) {
  if (value1 % value2 == 0) {
    return value1 / value2;
  } else {
    return (value1 / value2) + 1;
  }
}