# Routerino

## Introdução
O objetivo deste é desenvolver um roteador microcontrolado para fins didáticos, visando proporcionar uma compreensão aprofundada das redes de computadores no contexto da engenharia. O roteador será baseado no microcontrolador ATmega328P e utilizará a interface de rede ENC28J60, permitindo a implementação e o teste de diferentes protocolos de rede em um ambiente controlado.

## Funcionalidades e Componentes
O roteador microcontrolado será capaz de desempenhar as seguintes funcionalidades principais:

1. Encaminhamento de pacotes: O roteador será capaz de receber pacotes de dados de entrada e encaminhá-los para o destino apropriado, com base em informações de roteamento.

2. Tabelas de Roteamento: Será implementada uma tabela de roteamento que mapeará os destinos de rede para as interfaces de saída correspondentes.

3. Protocolos de Rede: Diferentes protocolos de rede, como o IP (Internet Protocol), serão implementados para permitir a comunicação entre diferentes dispositivos conectados à rede.

Os principais componentes do projeto incluem:

* Microcontrolador ATmega328P: O roteador será baseado no microcontrolador ATmega328P, que oferece capacidades de processamento e memória adequadas para executar os protocolos de rede necessários.

* Interface de Rede ENC28J60: Será utilizada a interface de rede ENC28J60 para permitir a conexão do roteador a outros dispositivos em uma rede local.

## Gravação do Projeto no Protótipo
Para gravar o projeto desenvolvido na etapa de prototipagem, será utilizado o gravador ASP, juntamente com a IDE do Arduino. O processo de gravação envolve os seguintes passos:

1. Preparação do ambiente: Certifique-se de que o gravador ASP esteja corretamente conectado ao protótipo do roteador (com microcontrolador ATmega328P e interface ENC28J60) e ao computador no qual a IDE do Arduino está instalada.

2. Configuração da IDE do Arduino: Selecione as configurações para tipo de programador USBasp na IDE do Arduino e o tipo de microcontrolador (ATmega328P).

3. Gravação do código no protótipo: Com as configurações corretas selecionadas, utilize a função de gravar utilizando um programador.

4. Verificação e depuração: Após a gravação, verifique se o protótipo está funcionando conforme o esperado.