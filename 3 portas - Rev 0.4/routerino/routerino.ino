#include "enc28j60.hpp"
#define debug 1

#define placa0 0
#define placa1 1
#define placa2 2

extern info tabela[3] = { { {0x00,0x00,0x00,0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, placa0, ERXST_INIT} 
                         ,{ {0x00,0x00,0x00,0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, placa1, ERXST_INIT}
                         ,{ {0x00,0x00,0x00,0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, placa2, ERXST_INIT}};

int len = 0, chip = 0;
unsigned char *packet;

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

  ENC28J60_Init(tabela[placa0].MAC, placa0);
  ENC28J60_Init(tabela[placa1].MAC, placa1);
  ENC28J60_Init(tabela[placa2].MAC, placa2);
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

int polling() {
  while (1)
  {
    if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa0) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 0 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa0));
      return placa0;
    }
    else if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa1) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 1 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa1));
      return placa1;
    }
    else if (ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa2) != 0)
    {
      if (debug)Serial.println();
      if (debug)Serial.print("Chip 2 - PACOTES:");
      if (debug)Serial.println(ENC28J60_Read_RCR(BANK1, OP_RCR | EPKTCNT, placa2));
      return placa2;
    }
    //if (debug)Serial.print(".");
    //if (debug)delay(1);
  }
}

void loop() {
  chip = polling();
  packet = ENC28J60_Packet_Receive(packet, &len, MAX_FRAMELEN, chip);

  if (chip == placa0) {
    ENC28J60_Send_Packet(packet, len, placa1);
    ENC28J60_Send_Packet(packet, len, placa2);
  }
  else if (chip == placa1) {
    ENC28J60_Send_Packet(packet, len, placa0);
    ENC28J60_Send_Packet(packet, len, placa2);
  }
  else if (chip == placa2) {
    ENC28J60_Send_Packet(packet, len, placa0);
    ENC28J60_Send_Packet(packet, len, placa1);
  }
  if (debug)ENC28J60_Print_packet(packet, len);
}
