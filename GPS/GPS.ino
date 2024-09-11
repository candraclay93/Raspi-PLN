#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Membuat instance untuk GPS dan serial port
TinyGPSPlus gps;

HardwareSerial GPS_Serial(1);  // Menggunakan UART1


void setup() {
  // Inisialisasi serial monitor untuk debugging
  Serial.begin(115200);
  
  // Inisialisasi serial GPS
  GPS_Serial.begin(9600, SERIAL_8N1, 18, 17);  // RX ke Pin 18, TX ke Pin 17

  Serial.println("Menginisialisasi GPS...");
}

void loop() {
  while (GPS_Serial.available() > 0) {
    char c = GPS_Serial.read();
    gps.encode(c); 
    if (gps.location.isValid()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    } else {
      Serial.println("Mencari sinyal GPS...");
    }
    if (gps.time.isValid()) {
      Serial.print("Waktu (UTC): ");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());
    }

    delay(1000);
  }
}
