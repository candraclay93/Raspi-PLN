#ifndef RTC_CONFIG_H
#define RTC_CONFIG_H

#include "pins_config.h"
#include <Wire.h>
#include <RTClib.h>

using PINS = PinsConfig;

class RTC : public RTC_DS3231 {
  public:
    // RTC();
    void rtcSetup();
    String getDateString();
    String getTimeString();
    uint64_t getTimestamp();

    void setDateTime(int year, int month, int day, int hour, int minute, int second);
  private:
    const int _RTC_SDA_PIN = PINS::RTC_SDA_PIN;
    const int _RTC_SCL_PIN = PINS::RTC_SCL_PIN;
};

#endif //RTC_CONFIG_H
