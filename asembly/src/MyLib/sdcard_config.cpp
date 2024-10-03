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

void SdCard::sdEnd() {
  spi.end();
}


void SdCard::writeFile(const char *path, const char *message) {
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Gagal membuka file untuk menulis");
    return;
  }
  file.println(message);
  // if (file.println(message)) {
  //   Serial.println("Berhasil menulis ke file");
  // } else {
  //   Serial.println("Gagal menulis ke file");
  // }
  file.close();
}

bool SdCard::append(const char *path, const char *message, bool first) {
File file = SD.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Gagal membuka file untuk menulis.");
    return false;
  }
  if ( first ) {
    file.println("1." + String(message));
  } else {
    file.println(String(message));
  }
  
  file.close();
  return true;
}


void SdCard::readFile(const char *path, int startLine,int endLine) {
  File file = SD.open(path);
  if (!file) {
    Serial.println("Gagal membuka file untuk membaca");
    return;
  }
  int currentIndex = 0;

  while (file.available()) {
    String line = file.readStringUntil('\n');
    

    int posisiTitik = line.indexOf('.');
    if (posisiTitik != -1) {
      currentIndex = (line.substring(0, posisiTitik)).toInt();
    } else {
      currentIndex++;
    }

    if (currentIndex >= startLine && currentIndex <= endLine) {
      Serial.println(line);
    }
    if (currentIndex > endLine) {
      break;
    }
  }
  file.close();
}

void SdCard::readLastLine(const char *path) {
  File file = SD.open(path, FILE_READ);
  if (!file) {
    Serial.println("Gagal membuka file untuk membaca");
    return;
  }
  String lastLine = "";

  file.seek(0, SeekEnd);
  int fileSize = file.size(); 

  for (int i = 0; i < fileSize; i++) {
    file.seek(fileSize - 1 - i);
    char c = file.read();
    if (c == '\n' && lastLine.length() > 0) {
      break;
    }
    lastLine = c + lastLine;
  }
  Serial.println(lastLine);
  file.close();
}

int SdCard::lastIndex(File file) {
  String lastLine = "";
  int lastIndex = 0;

  file.seek(0, SeekEnd);
  int fileSize = file.size(); 

  for (int i = 0; i < fileSize; i++) {
    file.seek(fileSize - 1 - i);
    char c = file.read();
    if (c == '\n' && lastLine.length() > 0) {
      break;
    }
    lastLine = c + lastLine;
  }

  int posisiTitik = lastLine.indexOf('.');
  if (posisiTitik != -1) {
    lastIndex = (lastLine.substring(0, posisiTitik)).toInt();
  } else {
    Serial.println("Titik tidak ditemukan dalam string.");
  }

  return lastIndex;
}

bool SdCard::writeLog(const char *path, const char *message, int limit){
  File file = SD.open(path, FILE_READ);
  if (!file) {
    append(path, message, true);
    Serial.println("file tidak di temukan.... membuat file baru");
    return false;
  }

  int lastIndex = this->lastIndex(file);

  if(lastIndex >= limit) {
    String newMessage = "1." + String(message);
    this->writeFile(path, newMessage.c_str());
  }else{
    String newMessage = String(lastIndex + 1) + "." + message;
    append(path, newMessage.c_str(), false);
  }

  file.close();
  return true;
}