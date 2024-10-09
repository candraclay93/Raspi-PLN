#include "sdcard_config.h"

SPIClass spi = SPIClass(1);

void SdCard::sdCardSetup() {
  spi.begin(_SPI_SCK, _SPI_MISO, _SPI_MOSI, _SD_CS);

  if (!SD.begin(_SD_CS, spi)) {
    Serial.println("Gagal menginisialisasi SD card");
    return;
  }
  Serial.println("SD card berhasil diinisialisasi");
}


void SdCard::writeFile(const char *path, const char *message) {
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Gagal membuka file untuk menulis");
    return;
  }
  if (file.println(message)) {
    Serial.println("Berhasil menulis ke file");
  } else {
    Serial.println("Gagal menulis ke file");
  }
  file.close();
}


void SdCard::readFile(const char *path) {
  File file = SD.open(path);
  if (!file) {
    Serial.println("Gagal membuka file untuk membaca");
    return;
  }
  Serial.println("Isi file");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}