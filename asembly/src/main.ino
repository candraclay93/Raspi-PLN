#include "base.h"
#include <ModbusMaster.h>

#define RS_RXD_PIN   16  
#define RS_TXD_PIN   15

ModbusMaster node;
Main base(115200);
SX127x LoRa;
HardwareSerial rsSerial(2);

char messageJson[300];
uint8_t counter = 0;
unsigned long previousMillis = 0;
unsigned long previousMillisSd = 0;

int totalPackets;
int packetSize = 50;


void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  rsSerial.begin(9600, SERIAL_8N1, RS_RXD_PIN, RS_TXD_PIN); 
  base.setup();
  xTaskCreate(
  readGps,
  "Read GPS Serial",
  1000,
  NULL,
  1,
  NULL  
  );

}


uint8_t getDataNode(byte slaveID, uint16_t address, uint16_t length) {
  node.begin(slaveID, rsSerial);
  uint8_t result;
  uint16_t data;
  uint16_t data1;

  return node.readHoldingRegisters(address, length);
  // if (result == node.ku8MBSuccess) {
    // data = node.getResponseBuffer(0);
  //   data1 = node.getResponseBuffer(1);
  //   Serial.print("Nilai dari register 1 di slave ID 2: ");
  //   Serial.println(data);
  //   Serial.println(data1);
  // } else {
  //   Serial.print("Error, kode: ");
  //   Serial.println(result);
  // }
}



void loop(){
  // writeSdLog();
  sendMessageLora();
  // getDataNode(2, 1, 0);
  Serial.println("Loop");
  delay(2000);
}

void writeSdLog(){
  if(millis() - previousMillisSd > 59999){
    base.sdEnd();
    delay(300);

    base.sdCardSetup();
    if(base.writeLog("/testlogg.txt", "Hello", 3000)){
      Serial.println("Write log success");
    }
    base.readLastLine("/testlogg.txt");
    previousMillisSd = millis();
  }
}

void sendMessageLora(){
  if(millis() - previousMillis > 30000){
    String v = String(base.getVoltage());
    String a = String(base.getCurrent());
    String p = String(base.getPower());
    String e = String(base.getEnergy());
    String f = String(base.getFrequency());
    String pf = String(base.getPf());
    String lat = String(base.getLatitude(), 6);
    String lng = String(base.getLongitude(), 6);
    String timestamp = String(base.getTimestamp());

    JsonDocument doc;
      doc["timestamp"] = timestamp;
      doc["id"] = base.chipID();

    JsonObject data = doc["data"].to<JsonObject>();
      data["v"] = v;
      data["a"] = a;
      data["p"] = p;
      data["e"] = e;
      data["f"] = f;
      data["pf"] = pf;
      data["lat"] = lat;
      data["lng"] = lng;
    serializeJson(doc, messageJson);

    int encodedLen = Base64.encodedLength(strlen(messageJson));
    char encodedData[encodedLen + 1];
    Base64.encode(encodedData, messageJson, strlen(messageJson));
    // encodedData[encodedLen] = '\0';

    totalPackets = base.ceil(strlen(encodedData), packetSize);

    Serial.println("Total packet: " + String(totalPackets));
    Serial.println("-----------------------------");

    for (int i = 0; i < totalPackets; i++) {
      sendPacket(i, encodedData);
      delay(1000);
    }
    previousMillis = millis();
  }
}

void sendPacket(int packetIndex, char encodedData[]){
  base.sdEnd();
  delay(300);

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

  int startByte = packetIndex * packetSize;
  int endByte = min(startByte + packetSize, (int)strlen(encodedData));

  // Serial.println("Start byte: " + String(startByte));
  // Serial.println("End byte: " + String(endByte));

  LoRa.beginPacket();
  LoRa.write(packetIndex + 1);
  LoRa.write(totalPackets);
  for (int i = startByte; i < endByte; i++) {   
    LoRa.write(encodedData[i]);
  }
  LoRa.write('\0');
  LoRa.endPacket();

  Serial.write(messageJson);
  Serial.println();

  Serial.print("Transmit time: ");
  LoRa.wait();

  Serial.print(LoRa.transmitTime());
  Serial.println(" ms");

  Serial.print("Paket "); Serial.print(packetIndex + 1); Serial.println(" dikirim!");
  Serial.println();

  LoRa.end();
  // base.sdCardSetup();
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    base.encode(GPS_Serial.read());
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}