#include "enc28j60.h"
#define debug 0

#define placa0 0
#define placa1 1
#define placa2 2

void setup() {
  if (debug)Serial.begin(230400);
  SPI_MasterBegin();
  ENC28J60_BlinkLEDs(500, placa0);
  ENC28J60_BlinkLEDs(500, placa1);
  ENC28J60_BlinkLEDs(500, placa2);

  if (debug)Serial.print("Chip 0 - Version1: 0x");
  if (debug)Serial.println(ENC28J60_Revision(placa0), HEX);
  if (debug)Serial.print("Chip 1 - Version2: 0x");
  if (debug)Serial.println(ENC28J60_Revision(placa1), HEX);
  if (debug)Serial.print("Chip 2 - Version2: 0x");
  if (debug)Serial.println(ENC28J60_Revision(placa2), HEX);
}

void loop(){}
