#include "enc28j60.hpp"

unsigned int NextPacketPtr[3] = {ERXST_INIT,ERXST_INIT,ERXST_INIT};

void ENC28J60_SetBank(unsigned char bank, int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_BFC | ECON1); //Limpando ECON1 para mudar de banco

  SPI_Write(ECON1_BSEL1 | ECON1_BSEL0);

  if (cs == 1)COM_end1;
  else COM_end2;

  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_BFS | ECON1); // Setando novo banco

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

void ENC28J60_Reset(int cs) {// System Reset Command (Soft Reset)
  if (cs == 1)COM_start1;
  else COM_start2;                

  SPI_Write(0xFF);

  if (cs == 1)COM_end1;
  else COM_end2;                 

  delay(52);
}

unsigned char ENC28J60_Read_One(unsigned char bank, unsigned char address, int cs) {
  unsigned char aux;
  
  ENC28J60_SetBank(bank, cs);
 
  if (cs == 1)COM_start1;
  else COM_start2;

  aux = SPI_Read_one(address, cs);
  
  if (cs == 1)COM_end1;
  else COM_end2;
  
  return aux;
}

unsigned char ENC28J60_Revision(int cs) {
  return ENC28J60_Read_One(BANK3, EREVID, cs);
}

void ENC28J60_Init(unsigned char *macaddr, int cs) {
  ENC28J60_Reset(cs);

  ENC28J60_SetBank(BANK0, cs);
  // Endereço de início do buffer de recebimento
  ENC28J60_Write(OP_WCR, ERXSTL, ERXST_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ERXSTH, ERXST_INIT >> 8, cs);
  //Copiando o endereço de inicio do buffer de recebimento para o registrador de ponteiro de leitura no buffer
  ENC28J60_Write(OP_WCR, ERXRDPTL, ERXST_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ERXRDPTH, ERXST_INIT >> 8, cs);
  // Endereço de fim do buffer de recebimento
  ENC28J60_Write(OP_WCR, ERXNDL, ERXND_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ERXNDH, ERXND_INIT >> 8, cs);

  // Endereço de início do buffer de transmissão
  ENC28J60_Write(OP_WCR, ETXSTL, ETXST_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ETXSTH, ETXST_INIT >> 8, cs);
  // Endereço de fim do buffer de transmissão
  ENC28J60_Write(OP_WCR, ETXNDL, ETXND_INIT & 0xFF, cs);
  ENC28J60_Write(OP_WCR, ETXNDH, ETXND_INIT >> 8, cs);

  //Filtros
  ENC28J60_SetBank(BANK1, cs);
  ENC28J60_Write(OP_WCR, ERXFCON, 0b00000000, cs); //Sem configurações de filtro

  ENC28J60_SetBank(BANK2, cs);
  // Habilitando o recebimento de pacotes pelo MAC
  // Pausar transmissão quando pacotes de controles são recebidos
  // Pacotes de controle de rede são descatados apos serem processados pelo MAC
  ENC28J60_Write(OP_WCR, MACON1, 0b00001101 , cs); // MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS
  
  //Habilitando um acrescimo para completar no mínimo 60 bytes para pacotes menores que isso e acréscimo de um CRC válido
  //Pacotes maiores que MAMXFL (Max frame lenght) não são enviados ou são descartados 
  //Operação em Full-Duplex 
  ENC28J60_Write(OP_WCR, MACON3, 0b00110001, cs); // MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX);

  // Configurando o maior pacote aceito pelo controlador
  // Nao faça pacotes maiores que MAX_FRAMELEN serão descartados
  ENC28J60_Write(OP_WCR, MAMXFLL, MAX_FRAMELEN & 0xFF, cs);
  ENC28J60_Write(OP_WCR, MAMXFLH, MAX_FRAMELEN >> 8, cs);

  //Configurando tempo entre pacotes, recomendado pelo IEEE para Full-duplex é 9.6s, Ethernet 10 Mbit/s
  ENC28J60_Write(OP_WCR, MABBIPG, 0x15, cs);
  
  // set inter-frame gap (non-back-to-back)
  ENC28J60_Write(OP_WCR, MAIPGL, 0x12, cs);
  ENC28J60_Write(OP_WCR, MAIPGH, 0x0C, cs);


  ENC28J60_SetBank(BANK3, cs);
  // write MAC address, NOTE: MAC address in ENC28J60 is byte-backward
  ENC28J60_Write(OP_WCR, MAADR5, macaddr[0], cs);
  ENC28J60_Write(OP_WCR, MAADR4, macaddr[1], cs);
  ENC28J60_Write(OP_WCR, MAADR3, macaddr[2], cs);
  ENC28J60_Write(OP_WCR, MAADR2, macaddr[3], cs);
  ENC28J60_Write(OP_WCR, MAADR1, macaddr[4], cs);
  ENC28J60_Write(OP_WCR, MAADR0, macaddr[5], cs);

  // Transmitted data will only be sent out on the twisted-pair interface
  // no loopback of transmitted frames
  ENC28J60_SetBank(BANK2, cs);
  ENC28J60_Write(OP_WCR, MIREGADR, PHCON2, cs);
  ENC28J60_Write(OP_WCR, MIWRL, 0x00, cs); 
  ENC28J60_Write(OP_WCR, MIWRH, 0x01, cs); 
  delay(15);


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
  if ((ENC28J60_Read_One(BANK0, EIR, cs) & EIR_TXERIF)) {
    ENC28J60_Write(OP_BFC, ECON1, ECON1_TXRTS, cs);
  }
}

unsigned char *ENC28J60_Continuos_Read_Buffer(int len, unsigned char *packet, int cs) {
  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(OP_RBM | RBM);

  for (int i = 0; i < len; i++) {
    *(packet + i) = SPI_Read(cs);
  }

  *(packet + len) = '\0';

  if (cs == 1)COM_end1;
  else COM_end2;

  return packet;
}

unsigned char *ENC28J60_Packet_Receive(unsigned char *packet, int *len, int maxlen, int cs) {

  int rxstat;

  ENC28J60_SetBank(BANK0, cs);
  ENC28J60_Write(OP_WCR, ERDPTL, (NextPacketPtr[cs - 1]), cs);
  ENC28J60_Write(OP_WCR, ERDPTH, (NextPacketPtr[cs - 1] >> 8), cs);

  NextPacketPtr[cs - 1]  = ENC28J60_Read_One(BANK0, OP_RBM | RBM, cs);
  NextPacketPtr[cs - 1] |= ENC28J60_Read_One(BANK0, OP_RBM | RBM, cs) << 8;

  *len  = ENC28J60_Read_One(BANK0, OP_RBM | RBM, cs);
  *len |= ENC28J60_Read_One(BANK0, OP_RBM | RBM, cs) << 8;
  *len -= 4; //remove the CRC count

  // limit retrieve length
  if (*len > maxlen - 1) {
    *len = maxlen - 1;
  }

  rxstat  = ENC28J60_Read_One(BANK0, OP_RBM | RBM, cs);
  rxstat |= ENC28J60_Read_One(BANK0, OP_RBM | RBM, cs) << 8;

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
    ENC28J60_Write(OP_WCR, ERXRDPTL, NextPacketPtr[cs - 1], cs);
    ENC28J60_Write(OP_WCR, ERXRDPTH, (NextPacketPtr[cs - 1] >> 8), cs);

    // decrement the packet counter indicate we are done with this packet
    ENC28J60_Write(OP_BFS, ECON2, ECON2_PKTDEC, cs);
  }
  return packet;
}
