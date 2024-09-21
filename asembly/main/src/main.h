#ifndef MAIN_H
#define MAIN_H
// extern HardwareSerial PzemSerial;
// extern HardwareSerial PzemSerial;
// #include "libs.h"
#include "rtc_config.h"
#include "pzem_config.h"
#include "sdcard_config.h"
#include "rs_config.h"
#include "gps_config.h"


void mySetup(const int serialBaudrate = 115200, bool serialEnable = true) {
  if (serialEnable) {
    Serial.begin(serialBaudrate);
  }
  rtcSetup();
  pzemSetup();
  sdCardSetup();
  gpsSetup();
}
// void rsLoop(void * parameter);
// void readGps(void * parameter);
#endif