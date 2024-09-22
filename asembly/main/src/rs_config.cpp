#include "rs_config.h"

HardwareSerial rsSerial(2);
ModbusRTU node;

RS485::RS485(int idSlave) {
  this->_idSlave = idSlave;
}

void RS485::rsInit() {
  rsSerial.begin(9600, SERIAL_8N1, _RS485_RX, _RS485_TX);
  node.begin(&rsSerial);
  node.slave(_idSlave);
}