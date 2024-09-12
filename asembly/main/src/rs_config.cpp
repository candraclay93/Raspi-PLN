#include "rs_config.h"


HardwareSerial rsSerial(2);
ModbusRTU node;

void rsInit() {
  rsSerial.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);
  node.begin(&rsSerial);
  node.slave(2);
}