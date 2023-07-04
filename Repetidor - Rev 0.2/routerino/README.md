# Versão 2 - Repetidor/Hub de Internet
Nesta versão do código, o protótipo atua como um repetidor/hub de internet, permitindo a replicação de pacotes de rede entre os controladores de rede. O objetivo é possibilitar a comunicação entre os dispositivos conectados às três portas de rede.

### Método de Polling e Replicação de Pacotes
O código utiliza o método de polling para verificar periodicamente se cada controlador de rede recebeu e armazenou um pacote de rede em seu buffer. Após identificar qual controlador recebeu o pacote, o código utiliza a comunicação SPI para transferir o pacote para o microcontrolador ATmega328P.

### Gravação e Replicação dos Pacotes
Uma vez que o ATmega328P recebe o pacote, ele é armazenado temporariamente na memória RAM. Em seguida, o pacote é replicado para os outros dois controladores de rede através da comunicação SPI. Dessa forma, os pacotes de rede são disseminados entre os dispositivos conectados, permitindo a comunicação eficiente entre eles.

### Funcionamento do Repetidor/Hub de Internet
Ao executar esse código, o protótipo funcionará como um repetidor/hub de internet. Os pacotes de rede destinados a um dispositivo conectado em uma das três portas serão replicados e enviados para os outros dois dispositivos. Isso possibilita que todos os dispositivos conectados tenham acesso aos pacotes destinados aos outros dispositivos.

*Lembrando que é necessário consultar o datasheet do ENC28J60 para obter informações específicas sobre os comandos e configurações necessárias para o correto funcionamento.*