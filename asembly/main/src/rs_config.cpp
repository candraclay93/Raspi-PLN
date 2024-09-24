#include "rs_config.h"

HardwareSerial rsSerial(2);

void RS485::rsInit(int idSlave) {
  rsSerial.begin(9600, SERIAL_8N1, _RS485_RX, _RS485_TX);
  this->begin(&rsSerial);
  this->slave(idSlave);
}