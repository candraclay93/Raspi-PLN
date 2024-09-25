#ifndef GPS_CONFIG_H
#define GPS_CONFIG_H

#include "pins_config.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

extern  EspSoftwareSerial::UART GPS_Serial;
using PINS = PinsConfig;

class GPS : public TinyGPSPlus{
  public:
    void gpsSetup();
    double getLatitude();
    double getLongitude();
    String getLocation(bool url = true);
  
  private:
    const int _GPS_RX = PINS::GPS_RX;
    const int _GPS_TX = PINS::GPS_TX;
};

#endif //GPS_CONFIG_H
