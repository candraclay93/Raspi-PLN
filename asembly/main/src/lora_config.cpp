#include "lora_config.h"

void loraSetup() {
    LoRa.setPins(CS, RESET, DIO0, DIO1, DIO3, DIO4);

  if (!LoRa.begin(433E6)) {
    Serial.println("Gagal memulai LoRa.");
    while (1);
  }
  Serial.println("LoRa berhasil dimulai!");
}


int receivePakage() {
  return packetSize = LoRa.parsePacket();
}

