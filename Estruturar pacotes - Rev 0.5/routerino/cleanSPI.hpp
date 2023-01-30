#include "Arduino.h"

void COM_start(int x);
void COM_end(int x);
void SPI_MasterBegin();
void SPI_Write(unsigned char data);
unsigned char SPI_Read(int cs);
unsigned char SPI_Read_one(unsigned char address, int cs);
