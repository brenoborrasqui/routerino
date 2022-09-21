//=====================================================Bibliotecas=================================================================================================

//=====================================================Definições==================================================================================================
#define nop __asm__("nop\n\t")              //Processador não execulta nada durante um ciclo (nop migrado do assembly)
#define debug 0                            //Váriavel para ligar ou deligar o debug via terminal

#define COM_start2  PORTB &= ~(1<<PB2)       //Inicia a comunicação SPI escrevendo 0 no pino 10
#define COM_end2    PORTB |=  (1<<PB2)       //Finaliza a comunicação SPI escrevendo 1 no pino 10

#define COM_start1  PORTB &= ~(1<<PB1)       //Inicia a comunicação SPI escrevendo 0 no pino 9
#define COM_end1    PORTB |=  (1<<PB1)       //Finaliza a comunicação SPI escrevendo 1 no pino 9
//==================================================Variáveis globais==============================================================================================

//=================================================Protótipo de funções============================================================================================
void SPI_MasterBegin();
void SPI_Write(unsigned char data);
void SPI_Message(unsigned char data);
unsigned char SPI_Read(unsigned char andress);

//=====================================================Funções=====================================================================================================
void SPI_MasterBegin() { //Configura os registradores do SPI
  //Configurando pinos
  DDRB = (1 << PB1) | (1 << PB2) | (1 << PB3) | (0 << PB4) | (1 << PB5); // PB5(MOSI)output, PB4(MISO)input, PB3(SCK)output, PB2(CS)output, PB1(CS)output
  PORTB |= (1 << PB1) | (1 << PB2); //Pino 9 e 10 em HIGH

  //Configurando SPI
  SPCR = (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (0 << SPR1) | (0 << SPR0);
  //(Ligando o SPI),(MSB primeiro),(Borda de subida do clock),(Fase do clock),(Velocidade do clock)
  /*SPR1  SPR0  CLOCK
    0     0     fosk/4
    0     1     fosk/16
    1     0     fosk/64
    1     1     fosk/128
  */
  nop;//Clock sem instrução para sincronização
  if (debug)Serial.begin(9600);
}

void SPI_Write(unsigned char data) {
  if (debug)Serial.println("Write:");
  if (debug)Serial.println(data, HEX);

  SPDR = data; //Grava o dado no registrador de comunicação
  while (!(SPSR & (1 << SPIF))); // Testa a flag de termino de trasmissão e aguarda se setada
}


unsigned char SPI_Read() {
  SPI_Write(0x00);
  if (debug)Serial.println("read:");
  if (debug)Serial.println(SPDR, HEX);
  return SPDR;
}

void SPI_Message(unsigned char *data, int cs ) { //Envia um vetor de bytes via SPI em pares
  int aux = 1;
  while (*data != NULL) {
    if (aux % 2 == 1) {
      if (cs == 1)COM_start1;
      else COM_start2;
      if (debug)Serial.println("Comunicação iniciada");
    }
    SPI_Write(*data);
    data++;
    if (aux % 2 == 0) {
      if (cs == 1)COM_end1;
      else COM_end2;
      if (debug)Serial.println("Comunicação finalizada");
    }
    aux++;
  }
}


unsigned char SPI_Read_one(unsigned char address, int cs) {

  if (cs == 1)COM_start1;
  else COM_start2;

  SPI_Write(address);

  SPI_Read();

  if (cs == 1)COM_end1;
  else COM_end2;

  if (debug)Serial.println("read:");
  if (debug)Serial.println(SPDR, HEX);

  return SPDR;
}
