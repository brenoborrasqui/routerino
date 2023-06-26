# Routerino

## Introdução
O objetivo do projeto é implementar um roteador microcontrolado para fins didáticos, visando proporcionar uma compreensão aprofundada das redes de computadores no contexto da engenharia. O roteador é baseado no microcontrolador ATmega328P e utilizará a interface de rede ENC28J60, permitindo a implementação e o teste de diferentes protocolos de rede em um ambiente controlado.

## Funcionalidades e Componentes
O roteador microcontrolado é capaz de desempenhar as seguintes funcionalidades principais:

1. Encaminhamento de pacotes: O roteador é capaz de receber pacotes de dados de entrada e encaminhá-los para o destino apropriado, com base em informações de roteamento.

2. Tabelas de Roteamento: É implementada uma tabela de roteamento que mapeará os destinos de rede para as interfaces de saída correspondentes.

3. Protocolos de Rede: Diferentes protocolos de rede, como a pilha TCP/IP, são implementados para permitir a comunicação entre diferentes dispositivos conectados à rede.

4. Outras funcionalidades: É possível utilizar o protótipo para outras aplicações utilizando os protocolos de rede em conjunto.

Os principais componentes do projeto incluem:

* Microcontrolador ATmega328P: O roteador é baseado nesse microcontrolador já conhecido pela placa de prototipagem Arduino UNO.

* Interface de Rede ENC28J60: É utilizado o mesmo chip nos shield's de rede comerciais para arduino, por isso um projeto viável.

## Gravação do Projeto no Protótipo
Para gravar o projeto desenvolvido na etapa de prototipagem, foi escolhido para fazer a gravção no prototipo o gravador ASP, juntamente com a IDE do Arduino. O processo de gravação envolve os seguintes passos:

1. Conexão do gravdor na placa: Certifique-se de que o gravador ASP esteja corretamente conectado no protótipo, seguindo o pinout do prototipo.

2. Configuração da IDE do Arduino: Selecione as configurações para tipo de programador USBasp na IDE do Arduino e o tipo de microcontrolador (ATmega328P).

3. Gravação do código no protótipo: Com as configurações corretas selecionadas, utilize a função de gravar utilizando um programador.

Pronto, já é possivel utilizar o prototipo com as versões de código disponível.