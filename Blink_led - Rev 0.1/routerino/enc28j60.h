#include "cleanSPI.h"

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

void ENC28J60_SetBank(unsigned char bank, int cs);
void ENC28J60_Write(unsigned char op, unsigned char address, unsigned char data, int cs);
void ENC28J60_BlinkLEDs(int ms, int cs);
void ENC28J60_Reset(int cs);
unsigned char ENC28J60_Read_RCR(unsigned char bank, unsigned char address, int cs);
unsigned char ENC28J60_Revision(int cs);

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

  ENC28J60_SetBank(bank, cs);
  aux = SPI_Read_one(address, cs);
  
  return aux;
}

unsigned char ENC28J60_Revision(int cs) {
  return ENC28J60_Read_RCR(BANK3, EREVID, cs);
}
