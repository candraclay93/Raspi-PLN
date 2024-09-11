#include <SPI.h>
#include <LoRa.h>

// Definisikan pin SPI dan pin CS (Chip Select)
#define SCK     12   // Pin SCK
#define MISO    13   // Pin MISO
#define MOSI    11   // Pin MOSI
#define CS      10    // Pin CS
#define RESET   0    // Pin RST, tidak terhubung atau diatur sebagai 0
#define DIO0    1   // Pin DIO0, jika perlu, disambungkan

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi SPI dan LoRa
  LoRa.setPins(CS, RESET, DIO0); // Tetapkan pin, RST dapat 0 jika tidak terhubung

  if (!LoRa.begin(433E6)) {  // Ganti 915E6 dengan frekuensi yang sesuai (misalnya 868E6 untuk Eropa)
    Serial.println("Gagal memulai LoRa.");
    while (1);
  }
  
  Serial.println("LoRa berhasil dimulai!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Paket diterima dengan ukuran: ");
    Serial.println(packetSize);

    // Baca payload
    String receivedMessage = "";
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }
    Serial.print("Pesan diterima: ");
    Serial.println(receivedMessage);
    
    Serial.print("RSSI: ");         // Print RSSI dari paket
    Serial.println(LoRa.packetRssi());
    Serial.println("---------------");
  }
  
  // Delay untuk memberi waktu penerimaan
  delay(1000); 
}
