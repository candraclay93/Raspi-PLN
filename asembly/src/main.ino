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

  // base.sdCardSetup();
  // base.sdCardSetup();
  // base.append("/testlogg.txt", "10.Hello", false);
  // base.writeFile("/testtlog.txt", "1.halo");
  // base.writeLog("/testlogg.txt", "Hello");
  // base.readFile("/testlogg.txt", 0, 20);
  // base.readFile("/testtlog.txt");
}


void getDataNode(byte slaveID, uint32_t address, uint16_t length) {
  node.begin(slaveID, rsSerial);
  uint8_t result;
  uint16_t data;
  uint16_t data1;

  result = node.readHoldingRegisters(1, 2);
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
  // writeSdLog();
  sendMessageLora();
  delay(2000);
  Serial.println("Looping");
}

void writeSdLog(){
  if(millis() - previousMillisSd > 59999){

    base.sdEnd();
    delay(300);

    base.sdCardSetup();
    if(base.writeLog("/testlogg.txt", "Hello", 3000)){
      Serial.println("Write log success");
    }
    // base.writeLog("/testlogg.txt", "Hello", 3000);
    base.readLastLine("/testlogg.txt");
    // Serial.println("Write log success");
    previousMillisSd = millis();
  }
}

void sendMessageLora(){
  if(millis() - previousMillis > 5000){
    Serial.println("Start LoRa");

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

    base.sdEnd();
    delay(300);

    if (!LoRa.begin(LORA_CS, LORA_RST, -1, -1, -1)){
      Serial.println("Something wrong, can't begin LoRa");
      while(1);
    }

    LoRa.setFrequency(433E6);
    LoRa.setTxPower(20, SX127X_TX_POWER_PA_BOOST);
    LoRa.setSpreadingFactor(10);
    LoRa.setBandwidth(52100);
    LoRa.setCodeRate(4);
    LoRa.setHeaderType(SX127X_HEADER_EXPLICIT);
    LoRa.setPreambleLength(12);
    LoRa.setPayloadLength(64);
    LoRa.setCrcEnable(true);
    LoRa.setSyncWord(0x34);

    int encodedLen = Base64.encodedLength(strlen(messageJson));
    char encodedData[encodedLen + 1];
    Base64.encode(encodedData, messageJson, strlen(messageJson));
    encodedData[encodedLen] = '\0';

    // Serial.println(encodedData);
    char message1[] = "eyJ0aW1lc3RhbXAiOiIxNzI3OTYzNTM5MDAwIiwiaWQiOiIxMjU4MzY0NCIsImRhdGE";
    char message2[] = "iOnsidiI6IjAuMDAiLCJhIjoiMC4wMCIsInAiOiIwLjAwIiwiZSI6IjAuMDAiLCJmIjo";
    char message3[] = "iMC4wMCIsInBmIjoiMC4wMCIsImxhdCI6IjAuMDAwMDAwIiwibG5nIjoiMC4wMDAwMDAifX0";

    

    LoRa.beginPacket();
    // LoRa.write(encodedData, sizeof(encodedData));
    LoRa.write(message1, sizeof(message1));
    // LoRa.write(message2, sizeof(message2));
    // LoRa.write(message3, sizeof(message3));
    LoRa.endPacket();

    Serial.write(messageJson);
    Serial.println();

    digitalWrite(LED_BUILTIN, HIGH);


    Serial.print("Transmit time: ");
    LoRa.wait();
    Serial.print(LoRa.transmitTime());

    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(" ms");
    Serial.println();

    LoRa.end();
    base.sdCardSetup();
    previousMillis = millis();
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