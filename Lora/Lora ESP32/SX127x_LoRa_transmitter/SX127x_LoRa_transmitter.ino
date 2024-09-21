#include <SX127x.h>
#include <ArduinoJson.h>


SX127x LoRa;

char message[] = "HeLoRa World!";
char message2[] = "Haii Dari ESP32 DEV";
uint8_t nBytes = sizeof(message);
uint8_t nBytes2 = sizeof(message2);
uint8_t counter = 0;


// #define SCK     18
// #define MISO    19
// #define MOSI    23
// #define CS      5
#define SCK     12
#define MISO    13
#define MOSI    11
#define CS      10
#define RESET   15    
// #define DIO0    2   


void setup() {

  Serial.begin(115200);

  Serial.println("Begin LoRa radio");
  if (!LoRa.begin(CS, RESET, -1, -1, -1)){
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

}

void loop() {
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
  // LoRa.write(jsonBuffer, size);
  // LoRa.write(message, nBytes);
  LoRa.write(jsonBuffer, size);
  LoRa.write(counter);
  LoRa.endPacket();

  Serial.write(jsonBuffer);
  Serial.println();

  // Serial.write(message);
  Serial.print("  ");
  Serial.println(counter++);

  LoRa.wait();

  Serial.print("Transmit time: ");
  Serial.print(LoRa.transmitTime());
  Serial.println(" ms");
  Serial.println();

  delay(5500);

}
