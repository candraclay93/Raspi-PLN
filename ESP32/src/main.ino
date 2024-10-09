#include "base.h"
#include <ModbusMaster.h>


ModbusMaster node;
Main base(115200);
SX127x LoRa;
HardwareSerial rsSerial(2);

char messageJson[300];
JsonDocument receiveMsg;

uint8_t counter = 0;
unsigned long previousMillis = 0;
unsigned long previousMillisSd = 0;
unsigned long previousMillisMb = 0;

int totalPackets;
int packetSize = 50;


void setup(){

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  rsSerial.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX); 
  base.setup(true);

  Serial.println(base.chipID());

  xTaskCreate(
  readGps,
  "Read GPS Serial",
  1000,
  NULL,
  1,
  NULL  
  );

  // xTaskCreate(
  // loraLoop,
  // "Read GPS Serial",
  // 4096,
  // NULL,
  // 1,
  // NULL  
  // );

}


uint8_t getDataNode(byte slaveID, uint16_t address, uint16_t length) {
  node.begin(slaveID, rsSerial);
  return node.readHoldingRegisters(address, length);
}



void loop(){
  // writeSdLog();
  // Serial.println(base.chipID());
  sendMessageLora();
  // modbusLoop();
  // Serial.println(base.getVoltage());

  // Serial.println("Loop");
  // delay(3000);
}

void modbusLoop(){
  if(millis() - previousMillisMb > 10000){
    uint8_t result;
    uint16_t data;

    result = getDataNode(2, 1, 1);

    if (result == node.ku8MBSuccess) {
    data = node.getResponseBuffer(0);
    Serial.print("Nilai dari register 1 di slave ID 2: ");
    Serial.println(data);
  } else {
    Serial.print("Error, kode: ");
    Serial.println(result);
  }
    previousMillisSd = millis();
  }
}

void writeSdLog(){
  if(millis() - previousMillisSd > 9999){
    base.myspiBegin();
    if (!SD.begin(5, MYSPI)) {
      Serial.println("Gagal menginisialisasi SD card");
      return;
    }

    delay(100);

    if(base.writeLog("/testlogg.txt", "Hello", 3000)){
      Serial.println("success");
    }
    Serial.println(base.readLastLine("/testlogg.txt"));

    base.myspiEnd();
    previousMillisSd = millis();
  }
}

void sendMessageLora(){
  if(millis() - previousMillis > 10000){
    base.myspiEnd();
    delay(300);

    LoRa.setSPI(MYSPI);
    if (!LoRa.begin(LORA_CS, LORA_RST, -1, -1,-1)){
      Serial.println("Something wrong, can't begin LoRa radio");
      return;
    }

    Serial.println("LoRa radio initialized");

    LoRa.setFrequency(433E6);
    LoRa.setTxPower(17, SX127X_TX_POWER_PA_BOOST);
    LoRa.setRxGain(SX127X_RX_GAIN_POWER_SAVING, SX127X_RX_GAIN_AUTO);
    LoRa.setSpreadingFactor(10);
    LoRa.setBandwidth(125000);
    LoRa.setCodeRate(4);
    LoRa.setHeaderType(SX127X_HEADER_EXPLICIT);
    LoRa.setPreambleLength(12);
    LoRa.setPayloadLength(64);
    LoRa.setCrcEnable(true);
    LoRa.setSyncWord(0x34);

    LoRa.request();
    LoRa.wait();

    uint8_t code = LoRa.read();
    if(code ==  211){
      return;
    }

    const uint8_t msgLen = LoRa.available() - 1;
    char message[msgLen];
    LoRa.read(message, msgLen);
    message[strlen(message) + 1] = '\0';


    int inputStringLength = sizeof(message);
    int decodedLength = Base64.decodedLength(message, inputStringLength);
    char decodedString[decodedLength];
    Base64.decode(decodedString, message, inputStringLength);

    DeserializationError error = deserializeJson(receiveMsg, decodedString);

    //     Serial.write(message, strlen(message));
    // Serial.print("RSSI: ");
    // Serial.print(LoRa.packetRssi());
    // Serial.print(" dBm | SNR: ");
    // Serial.print(LoRa.snr());
    // Serial.println(" dB");

    if (error) {
      Serial.println(error.f_str());
      return;
    }

    String fromReceive = receiveMsg["from"];
    String toReceive = receiveMsg["to"];
    String messageReceive = receiveMsg["data"]["msg"];

    // Serial.println(fromReceive);
    // Serial.println(toReceive);
    // Serial.println(messageReceive);

    if(toReceive != base.chipID() ) {
      Serial.println("id tidak cocok");
      return;
    }

    uint8_t status = LoRa.status();
    if (status == SX127X_STATUS_CRC_ERR) return;
    else if (status == SX127X_STATUS_HEADER_ERR) return;

    delay(100);
    
    String v = String(base.getVoltage());
    String a = String(base.getCurrent());
    String p = String(base.getPower());
    String e = String(base.getEnergy());
    String f = String(base.getFrequency());
    String pf = String(base.getPf());
    String lat = String(base.getLatitude(), 6);
    String lng = String(base.getLongitude(), 6);
    String timestamp = String(base.getTimestamp());
    String idcip = String(base.chipID());

    JsonDocument doc;
      doc["timestamp"] = timestamp;
      doc["from"] = idcip;
      doc["to"] = "9901";

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

    Serial.println(messageJson);
    totalPackets = base.ceil(strlen(encodedData), packetSize);

    Serial.println("Total packet: " + String(totalPackets));
    Serial.println("-----------------------------");

    for (int i = 0; i < totalPackets; i++) {
      sendPacket(i, encodedData);
      delay(1000);
    }

    LoRa.end();
    previousMillis = millis();
  }
}

void sendPacket(int packetIndex, char encodedData[]){
  Serial.println("test");
  int startByte = packetIndex * packetSize;
  int endByte = min(startByte + packetSize, (int)strlen(encodedData));

  LoRa.beginPacket();
  LoRa.write(211);
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
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    base.encode(GPS_Serial.read());
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}