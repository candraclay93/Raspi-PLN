#ifndef SDCARD_CONFIG_H
#define SDCARD_CONFIG_H

#include "pins_config.h"
#include "FS.h"
#include "SD.h"
// #include "SPI.h"



using PINS = PinsConfig;


class SdCard{
  public:
    // SdCard();
    // void sdCardSetup(SPIClass  spi);
    // void sdEnd(SPIClass  spi);
    void writeFile(const char *path, const char *message);
    bool append(const char *path, const char *message, bool first = false);
    void readFile(const char *path, int startLine = 0, int endLine = -1);
    String readLastLine(const char *path);

    int lastIndex(File file);
    bool writeLog(const char *path, const char *message, int limit = 1000);



  private:
    const int _SD_CS = PINS::SD_CS;

};

#endif //SDCARD_CONFIG_H
