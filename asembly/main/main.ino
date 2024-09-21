#include "src/main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"




void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  mySetup();
  rsInit();

  xTaskCreate(
  rsLoop,
  "RS485 Loop",
  1000,
  NULL,
  1,
  NULL  
  );

  xTaskCreate(
  readGps,
  "Read GPS Serial",
  1000,
  NULL,
  1,
  NULL  
  );

  node.addHreg(1, 1); 
  node.addHreg(2, 11); 
  node.addHreg(3, 12); 
  node.addHreg(4,100); 
}

void loop() {
  // Serial.println(getDateTimeString());
  // getAllPzem();
  // getLocation();

  // Serial.println("-------------------");
  int packetSize = LoRa.parsePacket();
  if(packetSize) {
    Serial.print("hello lora");
  }
  delay(1000);
}

void rsLoop (void * parameter) {
  while(true){
  node.task();
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void readGps(void * parameter){
  while(true){
    while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read()); // Mengirim data ke TinyGPS++
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
