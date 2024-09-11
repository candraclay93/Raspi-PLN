#include <SPI.h>
#include <LoRa.h>

// Definisikan pin SPI dan pin CS (Chip Select)
#define SCK     18   // Pin SCK
#define MISO    19   // Pin MISO
#define MOSI    23   // Pin MOSI
#define CS      5    // Pin CS
#define RESET   0    // Pin RST, tidak terhubung atau diatur sebagai 0
#define DIO0    2   // Pin DIO0, jika perlu, disambungkan

int count = 1;

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi SPI dan LoRa
  LoRa.setPins(CS, RESET, DIO0); // Tetapkan pin, RST dapat 0 jika tidak terhubung

  if (!LoRa.begin(433E6)) {
    Serial.println("Gagal memulai LoRa.");
    while (1);
  }
  
  Serial.println("LoRa berhasil dimulai!");
}

void loop() {
  Serial.println("Mengirim pesan..." + String(count) );
  
  LoRa.beginPacket();
  LoRa.print("Hello, LoRa!" + String(count));
  count++;
  LoRa.endPacket();
  
  delay(3000); // Tunggu 5 detik sebelum mengirim pesan berikutnya
}
