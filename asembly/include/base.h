#ifndef BASE_H
#define BASE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>

#include "pins_config.h"
#include "rtc_config.h"
#include "pzem_config.h"
#include "sdcard_config.h"
#include "gps_config.h"

using PINS = PinsConfig;

class Main : public RTC , public Pzem, public SdCard, public GPS
{
public:
    Main(long baudRate);
    void setup();
    String generateChipID();
    
private:
    long _baudRate;
};
#endif