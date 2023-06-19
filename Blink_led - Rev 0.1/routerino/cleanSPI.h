void COM_start(int x);
void COM_end(int x);
void SPI_MasterBegin();
void SPI_Write(unsigned char data);
unsigned char SPI_Read(int cs);
unsigned char SPI_Read_one(unsigned char address, int cs);

void SPI_MasterBegin() { //Configura os registradores do SPI
  //Configurando pinos
  DDRB = (1 << PB2) | (1 << PB3) | (0 << PB4) | (1 << PB5); // PB5(MOSI)output, PB4(MISO)input, PB3(SCK)output e PB2(CS)output
  DDRC = (1 << PC2) | (1 << PC3); // PC2(CS)output e PC3(CS)output
  PORTB |= (1 << PB2); // Definindo pino 10 como HIGH no início
  PORTC |= (1 << PC2) | (1 << PC3); // Definindo pino A2 e A3 como HIGH no início

  //Configurando SPI
  SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (0 << SPR1) | (0 << SPR0);
  SPSR = (1 << SPI2X);
  //(Ligando o SPI),(MSB primeiro),(Borda de subida do clock),(Fase do clock),(Velocidade do clock)
  /*SPI2X  SPR1  SPR0  CLOCK
      0     0     0    fosk/4
      0     0     1    fosk/16
      0     1     0    fosk/64
      0     1     1    fosk/128
      1     0     0    fosk/2
 */
}

void COM_start(int x) 
{
  if (x == 0) {
    PORTB &= ~(1<<PB2);  //Inicia a comunicação SPI escrevendo 0 no pino 8
  } else if (x == 1) {
    PORTC &= ~(1<<PC2);  //Inicia a comunicação SPI escrevendo 0 no pino 9
  } else {
    PORTC &= ~(1<<PC3);  //Inicia a comunicação SPI escrevendo 0 no pino 10
  }
}

void COM_end(int x) 
{
  if (x == 0) {
    PORTB |=  (1<<PB2);  //Finaliza a comunicação SPI escrevendo 1 no pino 8
  } else if (x == 1) {
    PORTC |=  (1<<PC2);   //Finaliza a comunicação SPI escrevendo 1 no pino 9
  } else {
    PORTC |=  (1<<PC3);   //Finaliza a comunicação SPI escrevendo 1 no pino 10
  }
}

void SPI_Write(unsigned char data) {
  SPDR = data; //Grava o dado no registrador de comunicação
  while (!(SPSR & (1 << SPIF))); // Testa a flag de termino de trasmissão e aguarda se setada
}

unsigned char SPI_Read(int cs) {
  SPI_Write(0x00);
  return SPDR;
}

unsigned char SPI_Read_one(unsigned char address, int cs) {
  COM_start(cs);

  SPI_Write(address);
  SPI_Read(cs);

  COM_end(cs);

  return SPDR;
}
