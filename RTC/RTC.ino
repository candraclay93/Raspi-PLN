#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

// Ganti dengan pin I2C yang kamu inginkan
#define SDA_PIN 8  // Contoh menggunakan GPIO 18 sebagai SDA
#define SCL_PIN 9  // Contoh menggunakan GPIO 19 sebagai SCL

void setup() {
  Serial.begin(115200);

  // Inisialisasi I2C dengan pin yang ditentukan
  Wire.begin(SDA_PIN, SCL_PIN);

  // Cek apakah RTC terhubung
  if (!rtc.begin()) {
    Serial.println("Tidak dapat menemukan RTC");
    while (1);
  }

  // Jika RTC kehilangan daya, set waktu ke waktu default
  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, mengatur ulang waktu!");
    // Set waktu ke waktu saat ini (kompilasi waktu)
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // Mendapatkan waktu saat ini dari RTC
  DateTime now = rtc.now();

  // Tampilkan waktu dan tanggal
  Serial.print("Tanggal: ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);

  Serial.print(" Waktu: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);

  Serial.println();

  delay(1000);  // Tunggu 1 detik sebelum memperbarui
}
