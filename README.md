Projeto StormSafe IoT

Descrição
O StormSafe é uma solução IoT que monitora variáveis ambientais críticas, como temperatura, umidade e chuva. O objetivo é detectar potenciais riscos de desastres urbanos, como alagamentos. O sistema usa um sensor DHT22 para temperatura e umidade, e os dados são enviados para o ThingSpeak para visualização em tempo real.

Componentes Utilizados
ESP32 (físico)

DHT22 (sensor de temperatura e umidade)

Potenciômetro (simulando sensor de chuva)

Botão (para ativar a coleta de dados)

ThingSpeak (para visualizar os dados em tempo real)

Arduino IDE (para programar o ESP32)

Objetivo
Monitorar as condições climáticas e ambientais em tempo real.

Enviar os dados para o ThingSpeak para monitoramento remoto.

Usar o botão para ativar a coleta de dados.

Configuração
Configuração do Hardware: O sensor DHT22, o potenciômetro e o botão são conectados ao ESP32, utilizando as portas apropriadas para garantir que a leitura dos dados seja realizada corretamente.

Configuração do Software: O código é carregado no ESP32 usando o Arduino IDE. Ele se conecta ao Wi-Fi, lê os dados dos sensores e envia para o ThingSpeak a cada 5 minutos.

Funcionamento
O sistema coleta dados de temperatura e umidade do sensor DHT22 e dados de chuva (simulados pelo potenciômetro).

A cada pressionamento do botão, o ESP32 envia os dados para o ThingSpeak.

A visualização dos dados pode ser acessada diretamente no ThingSpeak, onde você pode acompanhar em tempo real os dados coletados.

Testes
Verifique os Dados: Após a configuração e inicialização, pressione o botão para enviar os dados ao ThingSpeak. Os valores de temperatura, umidade e chuva serão atualizados no ThingSpeak.

Visualização: Os dados enviados são visualizados em tempo real na plataforma online.

Conclusão
O StormSafe IoT permite monitorar condições climáticas importantes, usando sensores simples e um ESP32. Ele fornece uma solução prática para acompanhar o risco de desastres urbanos, como alagamentos, em tempo real.
