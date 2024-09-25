#include "base.h"


#define RS_RXD_PIN   16  
#define RS_TXD_PIN   15

// ModbusMaster node;
Main base(115200);
SX127x LoRa;
HardwareSerial rsSerial(2);

char messageJson[300];
uint8_t counter = 0;



void setup(){
  rsSerial.begin(9600, SERIAL_8N1, RS_RXD_PIN, RS_TXD_PIN); 
  base.setup();

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

  xTaskCreate(
  readGps,
  "Read GPS Serial",
  1000,
  NULL,
  1,
  NULL  
  );
}


void getDataNode(byte slaveID, uint32_t address, uint16_t length) {
  node.begin(slaveID, rsSerial);
  uint8_t result;
  uint16_t data;
  uint16_t data1;

  result = node.readHoldingRegisters(address, length);
  if (result == node.ku8MBSuccess) {
    data = node.getResponseBuffer(0);
    data1 = node.getResponseBuffer(1);
    Serial.print("Nilai dari register 1 di slave ID 2: ");
    Serial.println(data);
    Serial.println(data1);
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
  String lat = String(base.getLatitude(), 6);
  String lng = String(base.getLongitude(), 6);
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
    data["lat"] = lat;
    data["lng"] = lng;

  
  serializeJson(doc, messageJson);
  int encodedLen = Base64.encodedLength(strlen(messageJson));
  char encodedData[encodedLen + 1];
  Base64.encode(encodedData, messageJson, strlen(messageJson));
  encodedData[encodedLen] = '\0';

  LoRa.beginPacket();
  LoRa.write(encodedData, sizeof(encodedData));
  LoRa.endPacket();

  Serial.write(messageJson);
  Serial.println();

  LoRa.wait();

  Serial.print("Transmit time: ");
  Serial.print(LoRa.transmitTime());
  Serial.println(" ms");
  Serial.println();

  // getDataNode(2, 0, 2);
  delay(5000);
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    base.encode(GPS_Serial.read());
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}