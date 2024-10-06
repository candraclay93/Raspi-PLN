#ifndef BASE_H
#define BASE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SX127x.h>
#include <Base64.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Wire.h>

#include "pins_config.h"
#include "rtc_config.h"
#include "pzem_config.h"
#include "sdcard_config.h"
#include "gps_config.h"

using PINS = PinsConfig;
extern SX127x LoRa;

#define SCK          PINS::SPI_SCK
#define MISO         PINS::SPI_MISO
#define MOSI         PINS::SPI_MOSI
#define LORA_CS      PINS::LORA_CS
#define LORA_RST     PINS::LORA_RST

class Main : public RTC , public Pzem, public SdCard, public GPS
{
public:
    Main(long baudRate);
    void setup();
    String generateChipID();
    char* encodePayload(char* inputString);
    int ceil(int value1, int value2);
    
private:
    long _baudRate;
};
#endif