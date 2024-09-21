#ifndef PZEM_CONFIG_H
#define PZEM_CONFIG_H

#include "pins_config.h"
#include <PZEM004Tv30.h>

extern PZEM004Tv30 pzem;

void pzemSetup();
float getVoltage();
float getCurrent();
float getPower();
float getEnergy();
float getFrequency();
float getPf();

#endif //PZEM_CONFIG_H
