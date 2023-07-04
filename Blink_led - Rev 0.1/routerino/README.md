# Versão 1 - Confirmação de Comunicação SPI entre ATmega328P e ENC28J60
Esta versão do código tem como objetivo verificar a correta comunicação SPI entre o microcontrolador ATmega328P e o módulo de interface de rede ENC28J60. A confirmação é feita através de uma verificação visual, onde os LEDs A e B dos módulos ENC28J60 piscam simultaneamente.

### Configuração dos Registradores
Para estabelecer a comunicação SPI entre o ATmega328P e o ENC28J60, os seguintes passos são realizados:

1. Configuração do ATmega328P: O microcontrolador é configurado como mestre SPI, taxa de clock (Fosc/2) e as demais configurações descritas no código.

2. Configuração do ENC28J60: O ENC28J60 é configurado como escravo SPI, e outras configurações seguindo as instruções fornecidas em seu datasheet.

### Piscar LEDs A e B do ENC28J60
Através do código gravado no ATmega328P, é possível enviar comandos via SPI para o ENC28J60, fazendo com que os LEDs A e B do ENC28J60 pisquem simultaneamente. O procedimento é realizado da seguinte maneira:

* Inicialização da comunicação SPI: O ATmega328P configura os registradores SPCR e SPSR para estabelecer a comunicação SPI com o ENC28J60.

* Envio de comandos: São enviados comandos específicos via SPI para o ENC28J60, instruindo-o a fazer o LED A e o LED B piscarem simultaneamente.

* Verificação da conclusão da transferência: Após o envio de cada comando, é verificado se a transferência SPI foi concluída, aguardando-se até que a transmissão seja finalizada.

* Ao executar esse código, é esperado que os LEDs A e B do ENC28J60 pisquem simultaneamente, confirmado assim a correta comunicação SPI entre o ATmega328P e o ENC28J60.

*Lembrando que é necessário consultar o datasheet do ENC28J60 para obter informações específicas sobre os comandos e configurações necessárias para o correto funcionamento.*