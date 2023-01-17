#include "Arduino.h"

#define COM_start2  PORTB &= ~(1<<PB2)       //Inicia a comunicação SPI escrevendo 0 no pino 10
#define COM_end2    PORTB |=  (1<<PB2)       //Finaliza a comunicação SPI escrevendo 1 no pino 10

#define COM_start1  PORTB &= ~(1<<PB1)       //Inicia a comunicação SPI escrevendo 0 no pino 9
#define COM_end1    PORTB |=  (1<<PB1)       //Finaliza a comunicação SPI escrevendo 1 no pino 9

void SPI_MasterBegin();
void SPI_Write(unsigned char data);
unsigned char SPI_Read(int cs);
unsigned char SPI_Read_one(unsigned char address, int cs);
