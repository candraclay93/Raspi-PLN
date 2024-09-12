#include "pzem_config.h"
// #include <PZEM004Tv30.h>


HardwareSerial PzemSerial(1);
PZEM004Tv30 pzem(PzemSerial, PZEM_RX, PZEM_TX);

void pzemSetup() {
  PzemSerial.begin(9600, SERIAL_8N1, PZEM_RX, PZEM_TX);
  Serial.println("Menginisialisasi PZEM004T");
}

void getVoltage() {
  float voltage = pzem.voltage();
  // Serial.print(voltage);
  if(isnan(voltage)) {
    Serial.println("Kesalahan membaca tegangan");
  } else {
    Serial.print("Tegangan: ");
    Serial.print(voltage);
    Serial.println(" V");
  }
}

void getCurrent() {
  float current = pzem.current();
  if(isnan(current)) {
    Serial.println("Kesalahan membaca arus");
  } else {
    Serial.print("Arus: ");
    Serial.print(current);
    Serial.println(" A");
  }
}

void getPower() {
  float power = pzem.power();
  if(isnan(power)) {
    Serial.println("Kesalahan membaca daya");
  } else {
    Serial.print("Daya: ");
    Serial.print(power);
    Serial.println(" W");
  }
}

void getEnergy() {
  float energy = pzem.energy();
  if(isnan(energy)) {
    Serial.println("Kesalahan membaca daya");
  } else {
    Serial.print("Daya: ");
    Serial.print(energy);
    Serial.println(" kWh");
  }
}

void getFrequency() {
  float frequency = pzem.frequency();
  if(isnan(frequency)) {
    Serial.println("Kesalahan membaca frekuensi");
  } else {
    Serial.print("Frekuensi: ");
    Serial.print(frequency);
    Serial.println(" Hz");
  }
}

void getPf(){
  float pf = pzem.pf();
  if(isnan(pf)) {
    Serial.println("Kesalahan membaca faktor daya");
  } else {
    Serial.print("Faktor Daya: ");
    Serial.println(pf);
  }
}

void getAllPzem() {
  getVoltage();
  getCurrent();
  getPower();
  getEnergy();
  getFrequency();
  getPf();
}