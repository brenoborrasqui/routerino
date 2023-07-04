# Versão 3 - Switch
Nesta versão do código, o protótipo funciona como um switch de rede, permitindo a conexão e comunicação entre dispositivos conectados às portas. Uma das principais adições é a implementação de uma tabela de endereços físicos MAC, que desempenha um papel importante na segurança dos pacotes trafegados.

### Tabela de Endereços MAC e Segurança dos Pacotes
O switch utiliza uma tabela de endereços físicos MAC para relacionar cada endereço com a respectiva porta do dispositivo conectado. Essa tabela é preenchida dinamicamente conforme os pacotes são recebidos e os endereços são observados. Com base nesses endereços, o switch é capaz de direcionar os pacotes diretamente para o dispositivo de destino, aumentando a segurança da rede.

### Funcionamento do Switch
Ao executar este código, o protótipo atuará como um switch de rede. Os pacotes serão direcionados para o dispositivo de destino com base na tabela de endereços MAC. Isso aumenta a segurança dos pacotes trafegados e melhora a eficiência da rede, minimizando o tráfego desnecessário.

*Lembre-se de consultar o datasheet do ENC28J60 para obter informações específicas sobre os comandos e configurações necessárias para o correto funcionamento.*