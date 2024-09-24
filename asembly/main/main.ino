#include "src/main.h"
#include <SX127x.h>
#include <ArduinoJson.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SCK     12
#define MISO    13
#define MOSI    11
#define CS_LORA 10
#define RESET   0   

Main base(115200);
SX127x LoRa;

uint8_t counter = 0;


void setup(){
  base.setup();

  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xFF) << i;
  }
  Serial.printf("ESP32 Chip ID: %u\n", chipId);

  if (!LoRa.begin(CS_LORA, RESET, -1, -1, -1)){
    Serial.println("Something wrong, can't begin LoRa");
    while(1);
  }
  LoRa.setFrequency(433E6);
  Serial.println("Set TX power to +17 dBm");
  LoRa.setTxPower(17, SX127X_TX_POWER_PA_BOOST);
  LoRa.setSpreadingFactor(10);
  LoRa.setBandwidth(125000);
  LoRa.setCodeRate(5);
  LoRa.setHeaderType(SX127X_HEADER_EXPLICIT);
  LoRa.setPreambleLength(12);
  LoRa.setPayloadLength(15);
  LoRa.setCrcEnable(true);
  LoRa.setSyncWord(0x34);

  Serial.println("\n-- LORA TRANSMITTER --\n");

  xTaskCreate(
  readGps,
  "Read GPS Serial",
  1000,
  NULL,
  1,
  NULL  
  );
}
void loop(){
    StaticJsonDocument<200> doc;
    doc["time"] = base.getDateString();
    doc["id"] = String(chipId);


  JsonObject data = doc.createNestedObject("data");
    data["v"] = 8;
    data["a"] = 7;
    data["a"] = 7;
    data["a"] = 7;

    char jsonBuffer[60];
    serializeJson(doc, jsonBuffer);
    uint8_t size = sizeof(jsonBuffer);

  LoRa.beginPacket();
  LoRa.write(jsonBuffer, size);
  LoRa.write(counter);
  LoRa.endPacket();

  Serial.write(jsonBuffer);
  Serial.println();

  // Serial.write(message);
  Serial.print("  ");
  Serial.println(counter++);

  LoRa.wait();

  Serial.println(base.getDateString());
  Serial.print("Transmit time: ");
  Serial.print(LoRa.transmitTime());
  Serial.println(" ms");
  Serial.println();

  delay(10000);
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    base.encode(GPS_Serial.read());
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

