#ifndef PZEM_CONFIG_H
#define PZEM_CONFIG_H

#include "pins_config.h"
#include <PZEM004Tv30.h>
// #include <HardwareSerial.h>

// extern HardwareSerial PzemSerial(1);
extern PZEM004Tv30 pzem;

void pzemSetup();
float getVoltage();
float getCurrent();
float getPower();
float getEnergy();
float getFrequency();
float getPf();
void getAllPzem();

#endif //PZEM_CONFIG_H
