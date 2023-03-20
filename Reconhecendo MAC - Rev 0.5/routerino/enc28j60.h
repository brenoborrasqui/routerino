typedef struct table
{
  unsigned char IP[4];
  unsigned char MAC[6];
  int placa;
  unsigned int NextPacketPtr;
}info;

extern info tabela[3];

#define OP_WCR    0x40 //Operador de escrita em registador de controle 
#define OP_RCR    0x00 //Operador de leitura em registador de controle
#define OP_WBM    0x60 //Operador de escrita no buffer
#define OP_RBM    0x20 //Operador de escrita no buffer
#define OP_BFS    0x80 //Escrita em apenas um bit
#define OP_BFC    0xA0 //Apagar apenas um bit

#define RBM       0x1A
#define WBM       0x1A

#define ECON1            0x1F //Registrador responsável pela mudança de bancos de registradores
#define ECON1_RXEN       0x04
#define ECON1_BSEL1      0x02
#define ECON1_BSEL0      0x01
#define ECON1_TXRTS      0x08

#define ECON2            0x1E
#define ECON2_PKTDEC     0x40

#define ESTAT            0x1D 
#define EIR              0x1C 
#define EIR_TXERIF       0x02 

#define BANK0   0x00
#define BANK1   0x01
#define BANK2   0x02
#define BANK3   0x03

#define MIREGADR  0x14 // Registrador de acesso ao MII
#define MIWRL     0x16 // MII Low byte
#define MIWRH     0x17 // MII High Byte

#define PHLCON1   0x00 // Registrador da memória PHY para acender led de configuração Full-Duplex
#define PHLCON    0x14 // Registrador da memoria PHY de controle dos LED's

#define EREVID    0x12 // Registrador de revisão do chip

#define ERXST_INIT   0x0000 //Início da memória do buffer de recebimento
#define ERXND_INIT  (0x1FFF-0x0600-1) //Fim da memória do buffer de recebimento
#define ETXST_INIT  (0x1FFF-0x0600) //Início da memoria do buffer de envio
#define ETXND_INIT   0x1FFF //fim da memoria do buffer de envio

#define EWRPTL    0x02
#define EWRPTH    0x03
#define ETXSTL    0x04
#define ETXSTH    0x05
#define ETXNDL    0x06
#define ETXNDH    0x07
#define ERXSTL    0x08
#define ERXSTH    0x09
#define ERXNDL    0x0A
#define ERXNDH    0x0B
#define ERXRDPTL  0x0C
#define ERXRDPTH  0x0D
#define ERXWRPTL  0x0E
#define ERXWRPTH  0x0F
#define EDMASTL   0x10
#define EDMASTH   0x11
#define EDMANDL   0x12
#define EDMANDH   0x13
#define EDMADSTL  0x14
#define EDMADSTH  0x15
#define EDMACSL   0x16
#define EDMACSH   0x17

#define ERXFCON          0x18 //Registrador responsável por filtros de recebimento de pacotes
#define ERXFCON_UCEN     0x80
#define ERXFCON_ANDOR    0x40
#define ERXFCON_CRCEN    0x20
#define ERXFCON_PMEN     0x10
#define ERXFCON_MPEN     0x08
#define ERXFCON_HTEN     0x04
#define ERXFCON_MCEN     0x02
#define ERXFCON_BCEN     0x01

#define EPMM0            0x08
#define EPMM1            0x09

#define EPMCSL           0x10
#define EPMCSH           0x11

#define MACON1           0x00 // Registradores MACON1

#define MACON2           0x01

#define MACON3           0x02 // Registradores MACON3

#define MABBIPG          0x04
#define MAIPGL           0x06
#define MAIPGH           0x07

#define MAMXFLL          0x0A
#define MAMXFLH          0x0B

#define MAADR1           0x00
#define MAADR0           0x01
#define MAADR3           0x02
#define MAADR2           0x03
#define MAADR5           0x04
#define MAADR4           0x05

#define EIE              0x1B 
#define EIE_INTIE        0x80
#define EIE_PKTIE        0x40

#define PHCON2           0x10
#define PHCON2_HDLDIS    0x0100

#define EPKTCNT          0x19

#define ERDPTL           0x00
#define ERDPTH           0x01

// Definindo maior tamanho de pacote aceito pelo ENC28J60 (MTU de rede é 1518 bytes):
#define MAX_FRAMELEN        1500

void ENC28J60_SetBank(unsigned char bank, int cs) {
  COM_start(cs);

  SPI_Write(OP_BFC | ECON1);

  SPI_Write(ECON1_BSEL1 | ECON1_BSEL0);

  COM_end(cs);

  COM_start(cs);

  SPI_Write(OP_BFS | ECON1);

  SPI_Write(bank);
  
  COM_end(cs);
}

void ENC28J60_Write(unsigned char op, unsigned char address, unsigned char data, int cs) {
  COM_start(cs);

  SPI_Write(op | address);

  SPI_Write(data);

  COM_end(cs);
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

void ENC28J60_Reset(int cs) {// System Reset Command (Soft Reset)
  COM_start(cs);               

  SPI_Write(0xFF);

  COM_end(cs);                

  delay(52);
}

unsigned char ENC28J60_Read_RCR(unsigned char bank, unsigned char address, int cs) {
  unsigned char aux;
  
  //COM_start(cs);

  ENC28J60_SetBank(bank, cs);
  aux = SPI_Read_one(address, cs);
  
  //COM_end(cs);
  
  return aux;
}

unsigned char ENC28J60_Revision(int cs) {
  return ENC28J60_Read_RCR(BANK3, EREVID, cs);
}

void ENC28J60_Init(int cs) {
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

  //Configurando tempo entre pacotes enviados pelo nosso dispositivo, recomendado pelo IEEE para Full-duplex é 9.6s, Ethernet 10 Mbit/s
  ENC28J60_Write(OP_WCR, MABBIPG, 0x15, cs);
  
  //Configuração de intervalo de tempo entre dois pacotes consecutivos enviados por dispositivos diferentes em uma rede, recomendações IEEE
  ENC28J60_Write(OP_WCR, MAIPGL, 0x12, cs);
  ENC28J60_Write(OP_WCR, MAIPGH, 0x0C, cs);

  //Configurando o endereço MAC
  //o endereço MAC deve ser escrito na memória do ENC28J60 em ordem inversa, ou seja, o primeiro byte do endereço 
  //deve ser escrito no último endereço de memória, o segundo byte deve ser escrito no penúltimo endereço de memória, e assim por diante.
  //Não será setado o MAC pois não tem nenhum filtro habilitado
  //ENC28J60_SetBank(BANK3, cs);
  //ENC28J60_Write(OP_WCR, MAADR5, macaddr[0], cs);
  //ENC28J60_Write(OP_WCR, MAADR4, macaddr[1], cs);
  //ENC28J60_Write(OP_WCR, MAADR3, macaddr[2], cs);
  //ENC28J60_Write(OP_WCR, MAADR2, macaddr[3], cs);
  //ENC28J60_Write(OP_WCR, MAADR1, macaddr[4], cs);
  //ENC28J60_Write(OP_WCR, MAADR0, macaddr[5], cs);

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
  COM_start(cs);

  SPI_Write(OP_WBM | WBM);
  for (int i = 0; i < len; i++)
  {
    SPI_Write(*data);
    data++;
  }

  COM_end(cs);
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
  COM_start(cs);

  SPI_Write(OP_RBM | RBM);

  for (int i = 0; i < len; i++) {
    *(packet + i) = SPI_Read(cs);
  }

  *(packet + len) = '\0';

  COM_end(cs);
  
  return packet;
}

unsigned char *ENC28J60_Packet_Receive(unsigned char *packet, int *len, int maxlen, int cs) {

  int rxstat;

  ENC28J60_SetBank(BANK0, cs);
  ENC28J60_Write(OP_WCR, ERDPTL, (tabela[cs].NextPacketPtr), cs);
  ENC28J60_Write(OP_WCR, ERDPTH, (tabela[cs].NextPacketPtr >> 8), cs);

  tabela[cs].NextPacketPtr  = ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs);
  tabela[cs].NextPacketPtr |= ENC28J60_Read_RCR(BANK0, OP_RBM | RBM, cs) << 8;

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
    ENC28J60_Write(OP_WCR, ERXRDPTL, tabela[cs].NextPacketPtr, cs);
    ENC28J60_Write(OP_WCR, ERXRDPTH, (tabela[cs].NextPacketPtr >> 8), cs);

    // decrement the packet counter indicate we are done with this packet
    ENC28J60_Write(OP_BFS, ECON2, ECON2_PKTDEC, cs);
  }
  return packet;
}
