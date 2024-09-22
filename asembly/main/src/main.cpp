#include "main.h"

// HardwareSerial PzemSerial(1);

Main::Main(long baudRate) : _baudRate(baudRate){}

void Main::setup() {
    Serial.begin(_baudRate);
    this->rtcSetup();
    this->pzemSetup();
    this->sdCardSetup();
    this->gpsSetup();
}

void Main::sendMessage(const String& message) {
    Serial.println(message);
}

String Main::readMessage() {
    String message = "";
    while (Serial.available() > 0) {
        char c = Serial.read();
        message += c;
    }
    return message;
}