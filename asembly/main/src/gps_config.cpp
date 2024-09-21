#include "gps_config.h"


TinyGPSPlus gps;
EspSoftwareSerial::UART GPS_Serial;

void gpsSetup() {
  GPS_Serial.begin(9600, SWSERIAL_8N1, GPS_RX, GPS_TX, false);
  if (!GPS_Serial) {
    Serial.println("Invalid GPS_Serial, check config"); 
    while (1) { 
      delay (1000);
    }
  } 
}

void getLocation() {
  if (gps.location.isUpdated()) {
    Serial.print("Latitude= "); 
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Longitude= ");
    Serial.println(gps.location.lng(), 6);
    Serial.println("-------------------");
    delay(1000);
  }else{
    Serial.println("Waiting for valid GPS data...");
    delay(1000);
  }
}
