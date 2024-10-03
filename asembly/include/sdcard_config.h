#ifndef SDCARD_CONFIG_H
#define SDCARD_CONFIG_H

#include "pins_config.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"


using PINS = PinsConfig;
extern SPIClass spi;


class SdCard{
  public:
    // SdCard();
    void sdCardSetup();
    void sdEnd();
    void writeFile(const char *path, const char *message);
    bool append(const char *path, const char *message, bool first = false);
    void readFile(const char *path, int startLine = 0, int endLine = -1);
    void readLastLine(const char *path);

    int lastIndex(File file);
    bool writeLog(const char *path, const char *message, int limit = 1000);



  private:
    const int _SD_CS = PINS::SD_CS;
    const int _SPI_MOSI = PINS::SPI_MOSI;
    const int _SPI_SCK  = PINS::SPI_SCK;
    const int _SPI_MISO = PINS::SPI_MISO;
};

#endif //SDCARD_CONFIG_H
