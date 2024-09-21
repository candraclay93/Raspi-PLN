#include "pzem_config.h"
// #include <PZEM004Tv30.h>


HardwareSerial PzemSerial(1);
PZEM004Tv30 pzem(PzemSerial, PZEM_RX, PZEM_TX);

void pzemSetup() {
  PzemSerial.begin(9600, SERIAL_8N1, PZEM_RX, PZEM_TX);
}

float getVoltage() {
  float voltage = pzem.voltage();
  return isnan(voltage) || voltage == 0 ? 0 : voltage;
}

float getCurrent() {
  float current = pzem.current();
  return isnan(current) || current == 0 ? 0 : current;
}

float getPower() {
  float power = pzem.power();
  return isnan(power) || power == 0 ? 0 : power;
}

float getEnergy() {
  float energy = pzem.energy();
  return isnan(energy) || energy == 0 ? 0 : energy;
}

float getFrequency() {
  float frequency = pzem.frequency();
  return isnan(frequency) || frequency == 0 ? 0 : frequency;
}

float getPf(){
  float pf = pzem.pf();
  return isnan(pf) || pf == 0 ? 0 : pf;
}
