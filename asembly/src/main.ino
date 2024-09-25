#include "base.h"
#include <ModbusMaster.h>
#include <SX127x.h>
#include <ArduinoJson.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SCK          12
#define MISO         13
#define MOSI         11
#define LORA_CS      10
#define LORA_RST     0 
#define RS_RXD_PIN   16  
#define RS_TXD_PIN   15

ModbusMaster node;
Main base(115200);
SX127x LoRa;
HardwareSerial rsSerial(2);

char messageJson[200];
uint8_t counter = 0;



void setup(){
  base.setup();
  rsSerial.begin(9600, SERIAL_8N1, RS_RXD_PIN, RS_TXD_PIN); 
  node.begin(2, rsSerial);
  loraSetup();


  xTaskCreate(
  readGps,
  "Read GPS Serial",
  1000,
  NULL,
  1,
  NULL  
  );
}

void loraSetup(){
  if (!LoRa.begin(LORA_CS, LORA_RST, -1, -1, -1)){
    Serial.println("Something wrong, can't begin LoRa");
    while(1);
  }
  LoRa.setFrequency(433E6);
  LoRa.setTxPower(17, SX127X_TX_POWER_PA_BOOST);
  LoRa.setSpreadingFactor(10);
  LoRa.setBandwidth(125000);
  LoRa.setCodeRate(4);
  LoRa.setHeaderType(SX127X_HEADER_EXPLICIT);
  LoRa.setPreambleLength(12);
  LoRa.setPayloadLength(64);
  LoRa.setCrcEnable(true);
  LoRa.setSyncWord(0x34);

  // LoRa.onTransmit(hello);
}

void getDataNode(){
  uint8_t result;
  uint16_t data;

  result = node.readHoldingRegisters(1, 1);
  if (result == node.ku8MBSuccess) {
    data = node.getResponseBuffer(0);
    Serial.print("Nilai dari register 1 di slave ID 2: ");
    Serial.println(data);
  } else {
    Serial.print("Error, kode: ");
    Serial.println(result);
  }
}



void loop(){

  String v = String(base.getVoltage());
  String a = String(base.getCurrent());
  String p = String(base.getPower());
  String e = String(base.getEnergy());
  String f = String(base.getFrequency());
  String pf = String(base.getPf());
  uint64_t timestamp = base.getTimestamp();

  JsonDocument doc;
    doc["timestamp"] = base.getTimestamp();
    doc["id"] = base.generateChipID();

  JsonObject data = doc["data"].to<JsonObject>();
    data["v"] = v;
    data["a"] = a;
    data["p"] = p;
    data["e"] = e;
    data["f"] = f;
    data["pf"] = pf;
    data["lat"] = "-7.0091521";
    data["lng"] = "110.441913";

  
  serializeJson(doc, messageJson);
  uint8_t size = sizeof(messageJson);

  LoRa.beginPacket();
  LoRa.write(messageJson, size);
  // LoRa.write();
  LoRa.endPacket();

  Serial.write(messageJson);
  // Serial.println();
  // Serial.print("  ");
  // Serial.println(timestamp);
  // Serial.println(base.getTimeString());
  // Serial.println(counter++);

  LoRa.wait();


  // Serial.print("Transmit time: ");
  // Serial.print(LoRa.transmitTime());
  // Serial.println(" ms");
  // Serial.println();

  getDataNode();

  delay(3000);
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    base.encode(GPS_Serial.read());
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


// void setup()
// {
// base.setup();
// }

// void loop()
// {
//   Serial.println("Hello World");
// }