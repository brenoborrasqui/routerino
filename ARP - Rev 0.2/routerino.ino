#include "enc28j60.hpp"
#define debug 1

int len = 0, chip = 0;
unsigned char *packet;

unsigned char MAC1[6] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
unsigned char MAC2[6] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

unsigned char teste1[42] = {0x7c, 0x8a, 0xe1, 0xdf, 0xfe, 0x9f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x45, 0x4c, 0x44, 0x45, 0x52, 0x21, 0xc0, 0xa8, 0x64, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char teste2[42] = {0x7c, 0x8a, 0xe1, 0xdf, 0xfe, 0x9f, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x45, 0x4c, 0x44, 0x45, 0x52, 0x21, 0xc0, 0xa8, 0x64, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {
  if (debug)Serial.begin(9600);
  SPI_MasterBegin();
  ENC28J60_BlinkLEDs(500, 1);
  ENC28J60_BlinkLEDs(500, 2);

  if (debug)Serial.print("Version1: 0x");
  if (debug)Serial.println(ENC28J60_Revision(1), HEX);
  if (debug)Serial.print("Version2: 0x");
  if (debug)Serial.println(ENC28J60_Revision(2), HEX);

  ENC28J60_Init(MAC1, 1);
  ENC28J60_Init(MAC2, 2);
}

void ENC28J60_Print_packet(unsigned char *packet, int len) {
  if (debug)Serial.println("");
  for (int i = 0; i < len; i++) {
    if (*(packet + i) == '/0') {
      if (debug)Serial.print("FIM");
      return 0;
    }
    if (debug)Serial.print(*(packet + i), HEX);
    if (debug)Serial.print("|");
  }
  if (debug)Serial.println();
}

//extern unsigned int NextPacketPtr;

int polling() {
  while (1)
  {
    if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, 1) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 1 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, 1));
      return 1;
    }
    if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, 2) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 2 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, 2));
      return 2;
    }
    if (debug)Serial.print(".");
    if (debug)delay(50);
    }
}

void loop() {
  chip = polling();
  packet = ENC28J60_Packet_Receive(packet, &len, MAX_FRAMELEN, chip);
  
  if(chip == 1){
    ENC28J60_Send_Packet(packet, len, 2);
  }
  else if(chip == 2){
    ENC28J60_Send_Packet(packet, len, 1);
  }


  if (debug)ENC28J60_Print_packet(packet, len);
}
