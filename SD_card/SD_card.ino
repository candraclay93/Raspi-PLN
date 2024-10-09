#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SD_CS 5  
#define SPI_MOSI 11
#define SPI_SCK  12
#define SPI_MISO 13

SPIClass spi = SPIClass(1);

void initSDCard() {
  spi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS); 

  if (!SD.begin(SD_CS, spi)) {
    Serial.println("Gagal menginisialisasi SD card!");
    return;
  }
  Serial.println("SD card berhasil diinisialisasi.");
}

void writeFile(const char *path, const char *message) {
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Gagal membuka file untuk menulis.");
    return;
  }
  if (file.println(message)) {
    Serial.println("Berhasil menulis ke file.");
  } else {
    Serial.println("Gagal menulis ke file.");
  }
  file.close();
}

void readFile(const char *path) {
  File file = SD.open(path);
  if (!file) {
    Serial.println("Gagal membuka file untuk membaca.");
    return;
  }

  Serial.println("Isi file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void setup() {
  Serial.begin(115200);
  
  initSDCard();

  writeFile("/example.txt", "Hello, custom SPI SD card!");
  
  readFile("/example.txt");
}

void loop() {
    writeFile("/example.txt", "Hello, custom SPI SD card!");
    delay(1000);
}