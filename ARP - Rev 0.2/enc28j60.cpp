#include "enc28j60.hpp"

//static unsigned int NextPacketPtr = ERXST_INIT;
unsigned int NextPacketPtr = ERXST_INIT;

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
  ENC28J60_Write(OP_WCR, MIWRL, 0x82, cs);
  ENC28J60_Write(OP_WCR, MIWRH, 0x38, cs);

  delay(ms);

  ENC28J60_Write(OP_WCR, MIREGADR, PHLCON, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x92, cs);
  ENC28J60_Write(OP_WCR, MIWRH, 0x39, cs);

  delay(ms);

  ENC28J60_Write(OP_WCR, MIREGADR, PHLCON, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x82, cs);
  ENC28J60_Write(OP_WCR, MIWRH, 0x38, cs);

  delay(ms);

  ENC28J60_Write(OP_WCR, MIREGADR, PHLCON, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x92, cs);
  ENC28J60_Write(OP_WCR, MIWRH, 0x39, cs);
}

void ENC28J60_Reset(int cs) {            // System Reset Command (Soft Reset)
  if (cs == 1)COM_start1;
  else COM_start2;                 // Enable

  SPI_Write(0xFF);

  if (cs == 1)COM_end1;
  else COM_end2;                 // Disable

  delay(52);
}

unsigned char ENC28J60_Read_RCR(unsigned char bank, unsigned char address, int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;

  ENC28J60_SetBank(bank, cs);
  return SPI_Read_one(address, cs);

  if (cs == 1)COM_end1;
  else COM_end2;
}

unsigned char ENC28J60_Revision(int cs) {
  return ENC28J60_Read_RCR(BANK3, EREVID, cs);
}

void ENC28J60_Init(unsigned char *macaddr, int cs) {
  ENC28J60_Reset(cs);

  //BANK0
  ENC28J60_SetBank(BANK0, cs);

  // Rx start
  ENC28J60_Write(OP_WCR, ERXSTL, ERXST_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ERXSTH, ERXST_INIT >> 8, cs);
  // set receive pointer address
  ENC28J60_Write(OP_WCR, ERXRDPTL, ERXST_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ERXRDPTH, ERXST_INIT >> 8, cs);
  // RX end
  ENC28J60_Write(OP_WCR, ERXNDL, ERXND_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ERXNDH, ERXND_INIT >> 8, cs);

  // TX start
  ENC28J60_Write(OP_WCR, ETXSTL, ETXST_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ETXSTH, ETXST_INIT >> 8, cs);
  // TX end
  ENC28J60_Write(OP_WCR, ETXNDL, ETXND_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ETXNDH, ETXND_INIT >> 8, cs);

  //BANK1
  ENC28J60_SetBank(BANK1, cs);

  //Filter
  ENC28J60_Write(OP_WCR, ERXFCON, 0b10110000, cs); //ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);//BROADCAST ...
  ENC28J60_Write(OP_WCR, EPMM0,  0x3f, cs);
  ENC28J60_Write(OP_WCR, EPMM1,  0x30, cs);
  ENC28J60_Write(OP_WCR, EPMCSL, 0xf9, cs);
  ENC28J60_Write(OP_WCR, EPMCSH, 0xf7, cs);

  // do bank 2 stuff
  ENC28J60_SetBank(BANK2, cs);

  // enable MAC receive
  ENC28J60_Write(OP_WCR, MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS, cs);

  // enable automatic padding to 60bytes and CRC operations
  ENC28J60_Write(OP_WCR, MACON3, 0b00110010, cs); //MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);

  // set inter-frame gap (non-back-to-back)
  ENC28J60_Write(OP_WCR, MAIPGL, 0x12, cs);
  ENC28J60_Write(OP_WCR, MAIPGH, 0x0C, cs);

  // set inter-frame gap (back-to-back)
  ENC28J60_Write(OP_WCR, MABBIPG, 0x12, cs);

  // Set the maximum packet size which the controller will accept
  // Do not send packets longer than MAX_FRAMELEN:
  ENC28J60_Write(OP_WCR, MAMXFLL, MAX_FRAMELEN & 0xFF, cs);
  ENC28J60_Write(OP_WCR, MAMXFLH, MAX_FRAMELEN >> 8, cs);

  // do bank 3 stuff
  ENC28J60_SetBank(BANK3, cs); //TROCA PARA BANCO 3

  // write MAC address
  // NOTE: MAC address in ENC28J60 is byte-backward
  ENC28J60_Write(OP_WCR, MAADR5, macaddr[0], cs);
  ENC28J60_Write(OP_WCR, MAADR4, macaddr[1], cs);
  ENC28J60_Write(OP_WCR, MAADR3, macaddr[2], cs);
  ENC28J60_Write(OP_WCR, MAADR2, macaddr[3], cs);
  ENC28J60_Write(OP_WCR, MAADR1, macaddr[4], cs);
  ENC28J60_Write(OP_WCR, MAADR0, macaddr[5], cs);

  // no loopback of transmitted frames
  ENC28J60_SetBank(BANK2, cs);
  ENC28J60_Write(OP_WCR, MIREGADR, PHCON2, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x00, cs); //PHCON2_HDLDIS);
  ENC28J60_Write(OP_WCR, MIWRH, 0x01, cs); //PHCON2_HDLDIS>>8);
  delay(15);

  // switch to bank 0
  ENC28J60_SetBank(BANK0, cs);
  // enable interrutps
  ENC28J60_Write(OP_WCR, EIE, EIE_INTIE | EIE_PKTIE, cs);
  // enable packet reception
  ENC28J60_Write(OP_BFS, ECON1, ECON1_RXEN, cs);
}

void ENC28J60_WriteBuffer(int len, unsigned char *data, int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_WBM | WBM);
  for (int i = 0; i < len; i++)
  {
    SPI_Write(*data);
    data++;
  }

  if (cs == 1)COM_end1;
  else COM_end2;
}

void ENC28J60_Send_Packet(unsigned char *data, int len, int cs) {

  ENC28J60_SetBank(BANK0, cs);

  // Set the write pointer to start of transmit buffer area
  ENC28J60_Write(OP_WCR, EWRPTL, ETXST_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, EWRPTH, ETXST_INIT >> 8, cs);

  // Set the TXND pointer to correspond to the packet size given
  ENC28J60_Write(OP_WCR, ETXNDL, (ETXST_INIT + len) & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ETXNDH, (ETXST_INIT + len) >> 8, cs);

  // write per-packet control byte (0x00 means use macon3 settings)
  ENC28J60_Write(OP_WBM, RBM, 0x00, cs);

  // copy the packet into the transmit buffer
  ENC28J60_WriteBuffer(len, data, cs);

  // send the contents of the transmit buffer onto the network
  ENC28J60_Write(OP_BFS, ECON1, ECON1_TXRTS, cs);
  if ((ENC28J60_Read_RCR(BANK0, EIR, cs) & EIR_TXERIF)) {
    ENC28J60_Write(OP_BFC, ECON1, ECON1_TXRTS, cs);
  }
}

unsigned char *ENC28J60_Continuos_Read_Buffer(int len, unsigned char *packet, int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_RBM | RBM);

  for (int i = 0; i < len; i++) {

    *(packet + i) = SPI_Read(cs);
    //Serial.println(*(packet + i));
  }

  *(packet + len) = '\0';

  if (cs == 1)COM_end1;
  else COM_end2;

  return packet;
}

unsigned char *ENC28J60_Packet_Receive(unsigned char *packet, int *len, int maxlen, int cs) {

  int rxstat;

  ENC28J60_SetBank(BANK0, cs);
  ENC28J60_Write(OP_WCR, ERDPTL, (NextPacketPtr), cs);
  ENC28J60_Write(OP_WCR, ERDPTH, (NextPacketPtr >> 8), cs);

  NextPacketPtr  = ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs);
  NextPacketPtr |= ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs) << 8;

  *len  = ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs);
  *len |= ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs) << 8;
  *len -= 4; //remove the CRC count

  // limit retrieve length
  if (*len > maxlen - 1) {
    *len = maxlen - 1;
  }

  rxstat  = ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs);
  rxstat |= ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs) << 8;

  if ((rxstat & 0x80) == 0) { //Pacote com defeito
    *len = 0;
  }
  else {                      //Pacote com ok
    free(packet);
    packet = calloc( ( (*len) + 1) , sizeof(unsigned char));

    packet = ENC28J60_Continuos_Read_Buffer((*len), packet, cs);

    //Move the RX read pointer to the start of the next received packet
    // This frees the memory we just read out
    ENC28J60_SetBank(BANK0, cs);
    ENC28J60_Write(OP_WCR, ERXRDPTL, NextPacketPtr, cs);
    ENC28J60_Write(OP_WCR, ERXRDPTH, (NextPacketPtr >> 8), cs);

    // decrement the packet counter indicate we are done with this packet
    ENC28J60_SetBank(BANK0, cs);
    ENC28J60_Write(OP_BFS, ECON2, ECON2_PKTDEC, cs);
  }
  return packet;
}
