#include "enc28j60.hpp"


void ENC28J60_SetBank(unsigned char bank, int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_BFC | ECON1);

  SPI_Write(ECON1_BSEL1 | ECON1_BSEL0);

  if (cs == 1)COM_end1;
  else COM_end2;

  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_BFS | ECON1);

  SPI_Write(bank);

  if (cs == 1)COM_end1;
  else COM_end2;
}

void ENC28J60_Write(unsigned char op, unsigned char address, unsigned char data, int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(op | address);

  SPI_Write(data);

  if (cs == 1)COM_end1;
  else COM_end2;
}

void ENC28J60_BlinkLEDs(int ms, int cs) {
  
  ENC28J60_SetBank(BANK2, cs);

  ENC28J60_Write(OP_WCR, MIREGADR, PHLCON, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x80, cs);// Acende LEDB - Laranja
  ENC28J60_Write(OP_WCR, MIWRH, 0x38, cs);// Acende LEDA - Verde

  delay(ms);

  ENC28J60_Write(OP_WCR, MIREGADR, PHLCON, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x90, cs);// Apaga LEDB - Laranja
  ENC28J60_Write(OP_WCR, MIWRH, 0x39, cs);// Apaga LEDA - Verde

  delay(ms);

  ENC28J60_Write(OP_WCR, MIREGADR, PHLCON, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x80, cs);// Acende LEDB - Laranja
  ENC28J60_Write(OP_WCR, MIWRH, 0x38, cs);// Acende LEDA - Verde

  delay(ms);

  ENC28J60_Write(OP_WCR, MIREGADR, PHLCON, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x90, cs);// Apaga LEDB - Laranja
  ENC28J60_Write(OP_WCR, MIWRH, 0x39, cs);// Apaga LEDA - Verde
}

void ENC28J60_Reset(int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;                

  SPI_Write(0xFF);

  if (cs == 1)COM_end1;
  else COM_end2;                 

  delay(52); // O datasheet tem uma revisão, la tem essa informação
}

unsigned char ENC28J60_Read_One(unsigned char bank, unsigned char address, int cs) {
  unsigned char aux;
  
  ENC28J60_SetBank(bank, cs);

  aux = SPI_Read_one(address, cs);
  
  return aux;
}

unsigned char ENC28J60_Revision(int cs) {
  return ENC28J60_Read_One(BANK3, EREVID, cs);
}

unsigned int ENC28J60_Read_MII(unsigned char address, int cs) {
  ENC28J60_SetBank(BANK2, cs);

  unsigned int data = 0x0000;
 
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_WCR | MIREGADR);
  SPI_Write(address);
  
  if (cs == 1)COM_end1;
  else COM_end2;

  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_WCR | MICMD);
  SPI_Write(0x01);

  if (cs == 1)COM_end1;
  else COM_end2;
  
  while((ENC28J60_Read_One(BANK3, MISTAT, cs) & MISTAT_BUSY) != 0)
  
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_WCR | MICMD);
  SPI_Write(0x00);

  if (cs == 1)COM_end1;
  else COM_end2;

  data = ENC28J60_Read_One(BANK2, MIRDL, cs);
  data |= ENC28J60_Read_One(BANK2, MIRDH, cs) << 8;
}
