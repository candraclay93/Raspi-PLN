#include <SPI.h>
#include <LoRa.h>

// Definisikan pin SPI dan pin CS (Chip Select)
// #define SCK     18   // Pin SCK
// #define MISO    19   // Pin MISO
// #define MOSI    23   // Pin MOSI
// #define CS      5    // Pin CS
#define SCK     12
#define MISO    13
#define MOSI    11
#define CS      10
#define RESET   0
#define DIO0    2

int counter = 0;

void setup() {
  Serial.begin(115200);
  // while (!Serial);
  LoRa.setPins(CS, RESET, DIO0);

  if (!LoRa.begin(433E6)) {  // Ganti 915E6 dengan frekuensi yang sesuai (misalnya 868E6 untuk Eropa)
    Serial.println("Gagal memulai LoRa.");
    while (1);
  }
  // LoRa.setSpreadingFactor(12);
  // LoRa.setCodingRate4(5);
  // LoRa.setSignalBandwidth(125000);
  // LoRa.enableCrc();
  // LoRa.setSyncWord(0x34);
  LoRa.setTxPower(17);
  
  Serial.println("LoRa berhasil dimulai!");
  LoRa.onTxDone(receiv);
}

void receiv(){
  Serial.println("hello tx");
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  Serial.println(LoRa.rssi());

  delay(5000);
}
