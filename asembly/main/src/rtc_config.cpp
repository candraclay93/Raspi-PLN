#include "rtc_config.h"


RTC_DS3231 rtc;
void rtcSetup() {
  Wire.begin(RTC_SDA_PIN, RTC_SCL_PIN);
  
  if (!rtc.begin()) {
    Serial.println("Tidak dapat menemukan RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, mengatur ulang waktu!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

String getDateTimeString() {
    DateTime now = rtc.now();
    char buffer[20];
    sprintf(buffer, "%04d/%02d/%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    delay(1000);
    return String(buffer);
}

void setDateTime(int year, int month, int day, int hour, int minute, int second) {
  DateTime newDateTime(year, month, day, hour, minute, second);
  rtc.adjust(newDateTime);
}