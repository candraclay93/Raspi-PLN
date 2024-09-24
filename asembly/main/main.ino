#include "src/main.h"
#include "src/rs_config.h"
#include <SX127x.h>
#include <ArduinoJson.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SCK       12
#define MISO      13
#define MOSI      11
#define LORA_CS   10
#define LORA_RST  0   

RS485 node;
Main base(115200);
SX127x LoRa;

char messageJson[200];
uint8_t counter = 0;

void hello(){
  Serial.print("hello");
}


void setup(){
  base.setup();
  node.rsInit(2);
  loraSetup();
  registerRS485();


  xTaskCreate(
  readGps,
  "Read GPS Serial",
  1000,
  NULL,
  1,
  NULL  
  );

  xTaskCreate(
  rsSlave,
  "Handle rs Slave",
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

  LoRa.onTransmit(hello);
}

void registerRS485(){
  node.addHreg(1, 1); 
  node.addHreg(2, 11); 
  node.addHreg(3, 12); 
  node.addHreg(4,100); 
}



void loop(){

  String v = String(base.getVoltage());
  String a = String(base.getCurrent());
  String p = String(base.power());
  String e = String(base.getEnergy());
  String f = String(base.getFrequency());
  String pf = String(base.getPf());
  uint64_t timestamp = base.getTimestamp();

  StaticJsonDocument<200> doc;
    doc["timestamp"] = base.getTimestamp();
    doc["id"] = base.generateChipID();

  JsonObject data = doc.createNestedObject("data");
    data["v"] = v;
    data["a"] = a;
    data["p"] = p;
    data["e"] = e;
    data["f"] = f;
    data["pf"] = pf;
    data["lat"] = "-7.0091521";
    data["lng"] = "110.441913";
  // StaticJsonDocument<200> doc;
  //   doc["timestamp"] = base.getTimestamp();
  //   doc["id"] = base.generateChipID();

  // JsonObject data = doc.createNestedObject("data");
  //   data["v"] = 200.0;
  //   data["a"] = 20.00;
  //   data["p"] = 10;
  //   data["e"] = 20.0;
  //   data["f"] = 50;
  //   data["pf"] = 3.0;
  //   data["lat"] = "-7.0091521";
  //   data["lng"] = "110.441913";

  
  serializeJson(doc, messageJson);
  uint8_t size = sizeof(messageJson);

  LoRa.beginPacket();
  LoRa.write(messageJson, size);
  // LoRa.write();
  LoRa.endPacket();

  Serial.write(messageJson);
  Serial.println();
  Serial.print("  ");
  Serial.println(timestamp);
  Serial.println(base.getTimeString());
  Serial.println(counter++);

  LoRa.wait();


  Serial.print("Transmit time: ");
  Serial.print(LoRa.transmitTime());
  Serial.println(" ms");
  Serial.println();

  delay(3000);
}

// char createJson(){
//   StaticJsonDocument<200> doc;
//     doc["timestamp"] = base.getTimestamp();
//     doc["id"] = base.generateChipID();

//   JsonObject data = doc.createNestedObject("data");
//     data["v"] = base.getVoltage();
//     data["a"] = base.getCurrent();
//     data["p"] = base.power();
//     data["e"] = base.getEnergy();
//     data["f"] = base.getFrequency();
//     data["pf"] = base.getPf();
//     data["pf"] = base.getPf();
//     data["lat"] = "-7.0091521";
//     data["lng"] = "110.441913";

  
//   serializeJson(doc, messageJson);
//   return messageJson;
// }



void rsSlave(void * parameter){
  while(true){
    node.task();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    base.encode(GPS_Serial.read());
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

