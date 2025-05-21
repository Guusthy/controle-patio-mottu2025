Projeto IoT com ESP32 (Wokwi) e Node-RED
- Objetivo
Desenvolver uma solução IoT simulada utilizando a plataforma Wokwi com ESP32, sensores ultrassônicos e LEDs, integrada com o Node-RED via protocolo MQTT para monitoramento em tempo real da distância medida por sensores.

- Tecnologias Utilizadas
ESP32 (via simulador Wokwi)

Sensores Ultrassônicos HC-SR04

LEDs (Verde e Vermelho)

Broker MQTT público (HiveMQ)

Node-RED (dashboard para visualização dos dados)

- Componentes e Conexões
Componentes utilizados:
1x ESP32

2x Sensores Ultrassônicos (HC-SR04)

1x LED Verde

1x LED Vermelho

Resistores (220Ω para LEDs)

Diagrama de ligação:
O diagrama de ligação foi criado digitalmente e se encontra no arquivo wiring_diagram.png incluso neste repositório.

- Funcionamento do Projeto
O ESP32 conecta-se à rede Wi-Fi simulada no Wokwi.

Dois sensores ultrassônicos medem a distância de obstáculos.

Os dados são publicados no tópico MQTT mottu/patio em formato JSON.

A lógica do LED funciona assim:

Se a distância do sensor principal for menor ou igual a 50 cm: LED verde acende.

Caso contrário: LED vermelho acende.

O Node-RED assina esse tópico e exibe os dados em um dashboard.

- Configuração MQTT
Broker: broker.hivemq.com

Porta: 1883

Tópico de Publicação: mottu/patio

Tópico de Inscrição (LED - ainda não utilizado): fiap/iot/led

ID MQTT: mottu_mqtt

- Fluxo Node-RED
O fluxo do Node-RED foi criado para:

Conectar-se ao broker MQTT.

Ler os dados do tópico mottu/patio.

Exibir as distâncias em gráficos e/ou campos de texto no dashboard.

(Opcional) Utilizar switches ou condições para acionar lógicas visuais com base nas distâncias.

Exemplo de configuração do nó MQTT IN:
Tópico: mottu/patio

QoS: 0

Broker: broker.hivemq.com:1883

- Teste no Wokwi
Como testar:
Acesse o link do projeto Wokwi: (inserir link aqui).

Clique em "Start Simulation".

Observe o monitor serial exibindo os valores de distância.

Verifique os LEDs acendendo conforme a distância.

Acesse o dashboard do Node-RED e visualize os dados em tempo real.
