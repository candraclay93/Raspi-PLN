#include <TinyGPS++.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TinyGPSPlus gps;

HardwareSerial GPS_Serial(2);  

void setup() {
  Serial.begin(115200);
  
  GPS_Serial.begin(9600, SERIAL_8N1, 18, 17);  

  Serial.println("Menginisialisasi GPS...");

    xTaskCreate(
    rsLoop,
    "RS485 Loop",
    1000,
    NULL,
    1,
    NULL  
    );
}

void loop() {
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);
      delay(1000);
      if (gps.time.isValid()) {
        Serial.print("Waktu (UTC): ");
        Serial.print(gps.time.hour());
        Serial.print(":");
        Serial.print(gps.time.minute());
        Serial.print(":");
        Serial.println(gps.time.second());
        delay(1000);
      }
        Serial.println("---------------");
    }
}

void rsLoop (void * parameter) {
  while(true){
    while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read()); 
    }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}