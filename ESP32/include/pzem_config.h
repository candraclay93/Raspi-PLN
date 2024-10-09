#ifndef PZEM_CONFIG_H
#define PZEM_CONFIG_H

#include <PZEM004Tv30.h>
#include "pins_config.h"

using PINS = PinsConfig;


class Pzem : public PZEM004Tv30 {
  public:

    Pzem();
    void pzemSetup();
    float getVoltage();
    float getCurrent();
    float getPower();
    float getEnergy();
    float getFrequency();
    float getPf();

  private:
    const int _PZEM_RX = PINS::PZEM_RX;
    const int _PZEM_TX = PINS::PZEM_TX;
};

#endif //PZEM_CONFIG_H
