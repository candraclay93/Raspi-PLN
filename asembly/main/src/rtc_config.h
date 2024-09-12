#ifndef RTC_CONFIG_H
#define RTC_CONFIG_H

#include "pins_config.h"
// #include "libs.h"
#include <Wire.h>
#include <RTClib.h>

extern RTC_DS3231 rtc;

void rtcSetup();
String getDateTimeString();
void setDateTime(int year, int month, int day, int hour, int minute, int second);

#endif //RTC_CONFIG_H
