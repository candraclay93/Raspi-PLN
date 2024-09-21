#include <SPI.h>
#include <LoRa.h>

// #define SCK     12
// #define MISO    13
// #define MOSI    11
// #define CS      10


#define SCK     18   // Pin SCK
#define MISO    19   // Pin MISO
#define MOSI    23   // Pin MOSI
#define CS      5    // Pin CS
#define RESET   15    // Pin RST, tidak terhubung atau diatur sebagai 0
#define DIO0    2   // Pin DIO0, jika perlu, disambungkan

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi SPI dan LoRa
  LoRa.setPins(CS, RESET, DIO0); // Tetapkan pin, RST dapat 0 jika tidak terhubung
 

  if (!LoRa.begin(433E6)) {  // Ganti 915E6 dengan frekuensi yang sesuai (misalnya 868E6 untuk Eropa)
    Serial.println("Gagal memulai LoRa.");
    while (1);
  }
  // LoRa.setSpreadingFactor(12);
  // LoRa.setCodingRate4(5);
  // LoRa.setSignalBandwidth(125000);
  // LoRa.enableCrc();/
  // LoRa.setSyncWord(0x34);
  
  Serial.println("LoRa berhasil dimulai!");

  // LoRa.onReceive(receiv());

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
  
  // Serial.println("loop");
  // Delay untuk memberi waktu penerimaan
  delay(1000); 
}
