#include "enc28j60.hpp"
#define debug 1

void setup() {
  if (debug)Serial.begin(230400);
  SPI_MasterBegin();
  ENC28J60_BlinkLEDs(500, 1);
  ENC28J60_BlinkLEDs(500, 2);

  if (debug)Serial.print("Version1: 0x");
  if (debug)Serial.println(ENC28J60_Revision(1), HEX);
  if (debug)Serial.print("Version2: 0x");
  if (debug)Serial.println(ENC28J60_Revision(2), HEX);

  delay(5000);

  if (debug)Serial.print("PHY Read 1: 0x");
  if (debug)Serial.println(ENC28J60_Read_MII(0x14, 1),HEX);
  if (debug)Serial.print("PHY Read 2: 0x");
  if (debug)Serial.println(ENC28J60_Read_MII(0x14, 2),HEX);
}

void loop()
{

}
