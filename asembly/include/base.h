#ifndef BASE_H
#define BASE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SX127x.h>
#include <Base64.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Wire.h>
// #include "SPI.h"


// extern SPIClass spi;

#include "serial_config.h"
#include "pins_config.h"
#include "rtc_config.h"
#include "pzem_config.h"
#include "sdcard_config.h"
#include "gps_config.h"

using PINS = PinsConfig;
extern SX127x LoRa;

#define MYSPI        SPI
#define SCK          PINS::SPI_SCK
#define MISO         PINS::SPI_MISO
#define MOSI         PINS::SPI_MOSI
#define LORA_CS      PINS::LORA_CS
#define LORA_RST     PINS::LORA_RST
#define RS485_RX     PINS::RS485_RX
#define RS485_TX     PINS::RS485_TX

class Main : public RTC , public Pzem, public SdCard, public GPS
{
public:
    Main(long baudRate);

    bool myspiBegin();
    void myspiEnd();
    void setup(bool debug = false);
    String chipID();
    char* encodePayload(char* inputString);
    int ceil(int value1, int value2);

    String decodePayload(char* inputString);
    
private:
    long _baudRate;
    const int _SPI_MOSI = PINS::SPI_MOSI;
    const int _SPI_SCK  = PINS::SPI_SCK;
    const int _SPI_MISO = PINS::SPI_MISO;
};
#endif