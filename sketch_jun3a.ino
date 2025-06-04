#include <WiFi.h>              // Biblioteca para conectar ao Wi-Fi
#include <HTTPClient.h>         // Biblioteca para realizar requisições HTTP
#include <ThingSpeak.h>         // Biblioteca para comunicação com o ThingSpeak
#include <Arduino_JSON.h>       // Biblioteca para manipulação de JSON

// Definição das credenciais de Wi-Fi
const char* SECRET_SSID = "Redmi Note 11S";      // Nome da rede Wi-Fi
const char* SECRET_PW = "18042006";             // Senha da rede Wi-Fi

WiFiClient client;                              // Cliente para comunicação via Wi-Fi

unsigned long channelID = 2979561;              // ID do canal no ThingSpeak
const char* writeAPIKey = "JTQWA409K8ROY3VS";   // Chave de escrita para o ThingSpeak
String apiKey = "e6f68c9c0ce990c80c96051f19ae1c67"; // API Key para acessar OpenWeatherMap

unsigned long lastTime = 0;                     // Variável para controlar o tempo da última requisição
unsigned long timerDelay = 300000;               // Intervalo entre as requisições (5 minutos)

String jsonBuffer;                              // Variável para armazenar a resposta JSON
double temperatura;                             // Variável para armazenar a temperatura
double umidade;                                 // Variável para armazenar a umidade

void setup() {
  Serial.begin(115200);                          // Inicializa a comunicação serial com a taxa de 115200 bps
  WiFi.mode(WIFI_STA);                           // Configura o modo do Wi-Fi para cliente
  ThingSpeak.begin(client);                      // Inicializa a comunicação com ThingSpeak
  Serial.println("Conectando ao WiFi...");       // Imprime no Serial Monitor
  conectarWiFi();                                // Chama a função para conectar ao Wi-Fi
}

void loop() {
  // Verifica se o tempo passado desde a última leitura é maior que o intervalo definido (5 minutos)
  if ((millis() - lastTime) > timerDelay) {

    // Verifica se o Wi-Fi está conectado
    if (WiFi.status() == WL_CONNECTED) {
      // Monta a URL para a API do OpenWeatherMap, incluindo a cidade e a chave da API
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=Sao%20Paulo,BR&APPID=" + apiKey + "&units=metric";

      // Faz uma requisição HTTP GET e armazena o resultado em jsonBuffer
      jsonBuffer = httpGETRequest(serverPath.c_str());

      // Converte a resposta JSON em um objeto JSON
      JSONVar myObject = JSON.parse(jsonBuffer);

      // Verifica se o JSON foi interpretado corretamente
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Erro ao interpretar JSON!");  // Caso o JSON seja inválido, imprime erro
        return;                                      // Retorna da função sem fazer mais nada
      }

      // Extrai a temperatura e a umidade do JSON
      temperatura = (double) myObject["main"]["temp"];
      umidade = (double) myObject["main"]["humidity"];

      // Imprime as informações no Serial Monitor
      Serial.println("Enviando dados ao ThingSpeak:");
      Serial.printf("Temperatura: %.1f °C\n", temperatura);
      Serial.printf("Umidade: %.0f %%\n", umidade);
      Serial.println("-----------------------------");

      // Define os campos do ThingSpeak com os valores de temperatura e umidade
      ThingSpeak.setField(1, (float)temperatura);
      ThingSpeak.setField(2, (float)umidade);

      // Envia os dados para o ThingSpeak e verifica o status
      int status = ThingSpeak.writeFields(channelID, writeAPIKey);

      // Se os dados foram enviados com sucesso, imprime no Serial Monitor
      if (status == 200) {
        Serial.println("Dados enviados com sucesso!\n");
      } else {
        // Caso contrário, imprime o erro
        Serial.print("Erro ao enviar: ");
        Serial.println(status);
      }
    } else {
      // Se o Wi-Fi não estiver conectado, tenta se reconectar
      Serial.println("WiFi desconectado. Tentando reconectar...");
      conectarWiFi();
    }

    lastTime = millis();  // Atualiza a variável lastTime com o tempo atual
  }
}

// Função para fazer uma requisição HTTP GET
String httpGETRequest(const char* serverName) {
  HTTPClient http;            // Cria um objeto HTTPClient para fazer a requisição
  http.begin(client, serverName);  // Inicializa a requisição com o URL do servidor
  int httpResponseCode = http.GET();  // Faz a requisição GET e armazena o código de resposta

  String payload = "{}";      // Inicializa a variável de payload com um JSON vazio

  if (httpResponseCode > 0) { // Se a resposta for positiva, pega o corpo da resposta
    payload = http.getString();
  } else {
    // Se houver um erro, imprime o código de erro
    Serial.print("Erro HTTP: ");
    Serial.println(httpResponseCode);
  }

  http.end();  // Finaliza a conexão HTTP
  return payload;  // Retorna o payload (dados recebidos)
}

// Função para conectar ao Wi-Fi
void conectarWiFi() {
  if (WiFi.status() != WL_CONNECTED) {   // Verifica se o Wi-Fi não está conectado
    Serial.print("Tentando conectar à rede: ");
    Serial.println(SECRET_SSID);  // Imprime o nome da rede Wi-Fi

    while (WiFi.status() != WL_CONNECTED) {  // Enquanto não estiver conectado, tenta conectar
      WiFi.begin(SECRET_SSID, SECRET_PW);  // Tenta conectar à rede
      delay(2000);  // Espera 2 segundos entre tentativas
      Serial.print(".");  // Imprime um ponto enquanto tenta conectar
    }

    Serial.println("\nWi-Fi conectado!");  // Imprime quando a conexão for bem-sucedida
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());  // Imprime o IP local do ESP32
  }
}
