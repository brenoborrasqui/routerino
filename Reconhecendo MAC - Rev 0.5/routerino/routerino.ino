#define debug 0

#define placa0 0
#define placa1 1
#define placa2 2

int len = 0, chip = 0;
unsigned char *packet;

#include "cleanSPI.h"
#include "enc28j60.h"
#include "handling.h"

ether *ptr;
ether *reply;

extern info tabela[3] = { { {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, placa0, ERXST_INIT}
  , { {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, placa1, ERXST_INIT}
  , { {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, placa2, ERXST_INIT}
};

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

  ENC28J60_Init(placa0);
  ENC28J60_Init(placa1);
  ENC28J60_Init(placa2);
}

void loop() {
  chip = polling();
  
  packet = ENC28J60_Packet_Receive(packet, &len, MAX_FRAMELEN, chip);
  
  arp_analysis(chip, packet);

  reply = (ether *)packet;

  chooseport(chip);

//    Serial.print("MAC SALVO EM INDICE 0:");
//  Serial.print(tabela[0].MAC[0], HEX);
//  Serial.print(tabela[0].MAC[1], HEX);
//  Serial.print(tabela[0].MAC[2], HEX);
//  Serial.print(tabela[0].MAC[3], HEX);
//  Serial.print(tabela[0].MAC[4], HEX);
//  Serial.println(tabela[0].MAC[5], HEX);
//
//  Serial.print("MAC SALVO EM INDICE 1:");
//  Serial.print(tabela[1].MAC[0], HEX);
//  Serial.print(tabela[1].MAC[1], HEX);
//  Serial.print(tabela[1].MAC[2], HEX);
//  Serial.print(tabela[1].MAC[3], HEX);
//  Serial.print(tabela[1].MAC[4], HEX);
//  Serial.println(tabela[1].MAC[5], HEX);
//
//  Serial.print("MAC SALVO EM INDICE 2:");
//  Serial.print(tabela[2].MAC[0], HEX);
//  Serial.print(tabela[2].MAC[1], HEX);
//  Serial.print(tabela[2].MAC[2], HEX);
//  Serial.print(tabela[2].MAC[3], HEX);
//  Serial.print(tabela[2].MAC[4], HEX);
//  Serial.println(tabela[2].MAC[5], HEX);
}
