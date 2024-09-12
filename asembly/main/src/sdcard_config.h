#ifndef SDCARD_CONFIG_H
#define SDCARD_CONFIG_H

#include "pins_config.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"


extern SPIClass spi;

void sdCardSetup();
void writeFile(const char *path, const char *message);
void readFile(const char *path);

#endif //SDCARD_CONFIG_H
