// #include "main.h"

// HardwareSerial PzemSerial(1);

// void mySetup(const int serialBaudrate, bool serialEnable) {
//   if (serialEnable) {
//     Serial.begin(serialBaudrate);
//   }

//   rtcSetup();
//   pzemSetup();
//   sdCardSetup();
//   gpsSetup();
// }

// void rsLoop (void * parameter) {
//   while(true){
//   node.task();
//   vTaskDelay(10 / portTICK_PERIOD_MS);
//   }
// }

// void readGps(void * parameter){
//   while(true){
//     while (GPS_Serial.available() > 0) {
//     gps.encode(GPS_Serial.read()); 
//     }
//   vTaskDelay(10 / portTICK_PERIOD_MS);
//   }
// }

