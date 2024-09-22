#include "src/main.h"
#include <SX127x.h>
#include <ArduinoJson.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SCK     12
#define MISO    13
#define MOSI    11
#define CS_LORA      10
#define RESET   0   

Main maini(115200);

SX127x LoRa;

char message[] = "HeLoRa World!";
char message2[] = "Haii Dari ESP32 DEV";
uint8_t nBytes = sizeof(message);
uint8_t nBytes2 = sizeof(message2);
uint8_t counter = 0;


void setup(){
  maini.setup();

    Serial.println("Begin LoRa radio");
  if (!LoRa.begin(CS_LORA, RESET, -1, -1, -1)){
    Serial.println("Something wrong, can't begin LoRa radio");
    while(1);
  }

  Serial.println("Set frequency to 433 Mhz");
  LoRa.setFrequency(433E6);

  Serial.println("Set TX power to +17 dBm");
  LoRa.setTxPower(17, SX127X_TX_POWER_PA_BOOST);                    // TX power +17 dBm using PA boost pin

  Serial.println("Set modulation parameters:\n\tSpreading factor = 7\n\tBandwidth = 125 kHz\n\tCoding rate = 4/5");
  LoRa.setSpreadingFactor(10);                                       // LoRa spreading factor: 7
  LoRa.setBandwidth(125000);                                        // Bandwidth: 125 kHz
  LoRa.setCodeRate(5);                                              // Coding rate: 4/5

  Serial.println("Set packet parameters:\n\tExplicit header type\n\tPreamble length = 12\n\tPayload Length = 15\n\tCRC on");
  LoRa.setHeaderType(SX127X_HEADER_EXPLICIT);                       // Explicit header mode
  LoRa.setPreambleLength(12);                                       // Set preamble length to 12
  LoRa.setPayloadLength(15);                                        // Initialize payloadLength to 15
  LoRa.setCrcEnable(true);                                          // Set CRC enable

  // Set syncronize word
  Serial.println("Set syncronize word to 0x34");
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
    doc["time"] = "2024/09/2";
    doc["id"] = "12345";


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

  Serial.println(maini.getDateString());
  Serial.print("Transmit time: ");
  Serial.print(LoRa.transmitTime());
  Serial.println(" ms");
  Serial.println();

  delay(5500);
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    maini.encode(GPS_Serial.read());
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

