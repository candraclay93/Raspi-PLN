#include <PZEM004Tv30.h>

// Tentukan pin UART yang digunakan
#define RXD2 47  // Pin RX untuk komunikasi dengan PZEM
#define TXD2 48  // Pin TX untuk komunikasi dengan PZEM

// Inisialisasi PZEM004Tv30 pada Serial2 (UART1)
PZEM004Tv30 pzem(Serial2, RXD2, TXD2);  // Gunakan Serial2 dan tentukan pin RX/TX

void setup() {
  // Inisialisasi komunikasi serial dengan kecepatan baud 9600 untuk PZEM-004T
  Serial.begin(115200);  // Serial monitor untuk debugging
  Serial2.begin(9600);   // Serial2 untuk komunikasi dengan PZEM

  Serial.println("Memulai komunikasi dengan PZEM-004T");
}

void loop() {
  // Membaca tegangan
  float voltage = pzem.voltage();
  if(isnan(voltage)) {
    Serial.println("Kesalahan membaca tegangan");
  } else {
    Serial.print("Tegangan: ");
    Serial.print(voltage);
    Serial.println(" V");
  }

  // Membaca arus
  float current = pzem.current();
  if(isnan(current)) {
    Serial.println("Kesalahan membaca arus");
  } else {
    Serial.print("Arus: ");
    Serial.print(current);
    Serial.println(" A");
  }

  // Membaca daya aktif
  float power = pzem.power();
  if(isnan(power)) {
    Serial.println("Kesalahan membaca daya");
  } else {
    Serial.print("Daya: ");
    Serial.print(power);
    Serial.println(" W");
  }

  // Membaca energi (kWh)
  float energy = pzem.energy();
  if(isnan(energy)) {
    Serial.println("Kesalahan membaca energi");
  } else {
    Serial.print("Energi: ");
    Serial.print(energy);
    Serial.println(" kWh");
  }

  // Membaca frekuensi
  float frequency = pzem.frequency();
  if(isnan(frequency)) {
    Serial.println("Kesalahan membaca frekuensi");
  } else {
    Serial.print("Frekuensi: ");
    Serial.print(frequency);
    Serial.println(" Hz");
  }

  // Membaca faktor daya
  float pf = pzem.pf();
  if(isnan(pf)) {
    Serial.println("Kesalahan membaca faktor daya");
  } else {
    Serial.print("Faktor Daya: ");
    Serial.println(pf);
  }

  Serial.println("---------------------");

  delay(2000);  // Tunggu 2 detik sebelum pembaruan berikutnya
}
