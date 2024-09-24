#include "main.h"

// HardwareSerial PzemSerial(1);
int chipId = 0;
Main::Main(long baudRate) : _baudRate(baudRate){}

void Main::setup() {
    Serial.begin(_baudRate);
    this->rtcSetup();
    this->pzemSetup();
    this->sdCardSetup();
    this->gpsSetup();
}

String Main::generateChipID(){
    for (int i = 0; i < 17; i = i + 8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xFF) << i;
    }
    return String(chipId);
}