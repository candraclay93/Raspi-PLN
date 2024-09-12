#ifndef GPS_CONFIG_H
#define GPS_CONFIG_H

#include "pins_config.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

extern TinyGPSPlus gps;
extern EspSoftwareSerial::UART GPS_Serial;

void gpsSetup();
void getLocation();
// // bool gpsTimeValid();
// // void getLatitude();
// // void getLongitude();
// // void getAltitude();
// // void getSatellites();
// // void getTime();
// // void getSpeed();
// // void getCourse();

#endif //GPS_CONFIG_H

