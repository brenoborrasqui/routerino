#include "cleanSPI.hpp"

#define OP_WCR    0x40 //Write Control Register
#define OP_RCR    0x00 //Read Control Register
#define OP_WBM    0x60 //Write Buffer Memory
#define OP_RBM    0x20 //Read Buffer Memory
#define OP_BFS    0x80 //Bit Fild Set
#define OP_BFC    0xA0 //Bit Fild Set

#define RBM       0x1A
#define WBM       0x1A

#define ECON1            0x1F //used to control the main functions of the ENC28J60.
#define ECON1_RXEN       0x04
#define ECON1_BSEL1      0x02
#define ECON1_BSEL0      0x01
#define ECON1_TXRTS      0x08

#define ESTAT            0x1D // breno vai usar
#define EIR              0x1C // breno vai usar
#define EIR_TXERIF       0x02 // breno vai usar

#define BANK0   0x00
#define BANK1   0x01
#define BANK2   0x02
#define BANK3   0x03

#define MIREGADR  0x14 // MII Register Address
#define MIWRL     0x16 // MII Write Data Low Byte
#define MIWRH     0x17 // MII Write Data High Byte

#define PHLCON    0x14 // PHY MODULE LED CONTROL REGISTER 0011 0100 0010 001x

#define EREVID    0x12 // Revision Information

#define ERXST_INIT   0x0000
#define ERXND_INIT   (0x1FFF-0x0600-1)
#define ETXST_INIT   (0x1FFF-0x0600)
#define ETXND_INIT   0x1FFF

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

#define ERXFCON          0x18
// ENC28J60 ERXFCON Register Bit Definitions(Receive Filters)
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

#define MACON1           0x00
// ENC28J60 MACON1 Register Bit Definitions
#define MACON1_LOOPBK    0x10
#define MACON1_TXPAUS    0x08
#define MACON1_RXPAUS    0x04
#define MACON1_PASSALL   0x02
#define MACON1_MARXEN    0x01

#define MACON2           0x01

#define MACON3           0x02
// ENC28J60 MACON3 Register Bit Definitions
#define MACON3_PADCFG2   0x80
#define MACON3_PADCFG1   0x40
#define MACON3_PADCFG0   0x20
#define MACON3_TXCRCEN   0x10
#define MACON3_PHDRLEN   0x08
#define MACON3_HFRMLEN   0x04
#define MACON3_FRMLNEN   0x02
#define MACON3_FULDPX    0x01

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
// ENC28J60 EIE Register Bit Definitions
#define EIE_INTIE        0x80
#define EIE_PKTIE        0x40

#define PHCON2           0x10
// ENC28J60 PHY PHCON2 Register Bit Definitions
#define PHCON2_HDLDIS    0x0100

#define EPKTCNT          0x19

#define ERDPTL           0x00
#define ERDPTH           0x01

#define ECON2            0x1E
#define ECON2_PKTDEC     0x40

// max frame length which the conroller will accept(note: maximum ethernet frame length would be 1518):
#define MAX_FRAMELEN        1500

void ENC28J60_SetBank(unsigned char bank, int cs);
void ENC28J60_Write(unsigned char op, unsigned char address, unsigned char data, int cs);
void ENC28J60_BlinkLEDs(int ms, int cs);
void ENC28J60_Reset(int cs);
unsigned char ENC28J60_Read_RCR(unsigned char bank, unsigned char address, int cs);
unsigned char ENC28J60_Revision(int cs);

void ENC28J60_Init(unsigned char *macaddr, int cs);

void ENC28J60_WriteBuffer(int len, unsigned char *data, int cs);
void ENC28J60_Send_Packet(unsigned char *data, int len, int cs);

unsigned char *ENC28J60_Continuos_Read_Buffer(int len, unsigned char *packet, int cs);
unsigned char *ENC28J60_Packet_Receive(unsigned char *packet, int *len, int maxlen, int cs);
