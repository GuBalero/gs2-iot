#include <ESP32Servo.h>
#include <WiFi.h>  
#include <PubSubClient.h>

Servo servo1;  // Cria um objeto do tipo Servo para controlar o servomotor 1
Servo servo2;  // Cria um objeto do tipo Servo para controlar o servomotor 2
Servo servo3;  // Cria um objeto do tipo Servo para controlar o servomotor 3

const int servoPin1 = 17;  // Pino ao qual o servomotor 1 está conectado
const int servoPin2 = 18;  // Pino ao qual o servomotor 2 está conectado
const int servoPin3 = 16;  // Pino ao qual o servomotor 3 está conectado

const char* ssid = "Wokwi-GUEST"; // SSID da rede Wi-Fi
const char* password = ""; // Senha da rede Wi-Fi
const char* mqtt_server = "test.mosquitto.org"; // URL do servidor MQTT
WiFiClient espClient; // Cria um objeto WiFiClient chamado espClient para gerenciar a conexão Wi-Fi
PubSubClient client(espClient); // Cria um objeto PubSubClient chamado client para trabalhar com MQTT usando espClient como cliente de rede MQTT

void setup_wifi() { 
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid); // Exibe o SSID da rede Wi-Fi no monitor serial

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password); // Inicia a conexão Wi-Fi com as credenciais especificadas

  while (WiFi.status() != WL_CONNECTED) { 
    // Enquanto o status da conexão Wi-Fi não estiver como "WL_CONNECTED" (ou seja, não estiver conectado),
    // o programa continuará neste loop aguardando a conexão ser estabelecida.
    // Isso é necessário para garantir que o dispositivo ESP32 esteja conectado à rede Wi-Fi antes de prosseguir.
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); // Exibe a mensagem quando a conexão Wi-Fi é estabelecida

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Exibe o endereço IP local atribuído ao dispositivo
}

void reconnect() { 
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection..."); // Tentando estabelecer a conexão MQTT
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX); // Gera um ID de cliente MQTT aleatório

    if (client.connect(clientId.c_str())) {
      client.subscribe("/gubalero/gs", 0);
      Serial.println("Connected"); // Exibe a mensagem quando a conexão MQTT é bem-sucedida
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state()); // Exibe o estado de conexão MQTT
      Serial.println(" try again in 5 seconds"); // Exibe uma mensagem de falha e tenta novamente após 5 segundos
      delay(5000);
    }}
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Esta função é chamada quando uma mensagem MQTT é recebida no tópico especificado

  // Converte o primeiro caractere do payload para um número inteiro
  int openServo = (char)payload[0] - '0';

  // Com base no valor de openServo, move o servo correspondente
  if(openServo == 0){
    // Move o servo1 para a posição 180 graus por 2 segundos e, em seguida, retorna para 0 graus
    servo1.write(180);
    delay(2000);
    servo1.write(0);
  }
  else if(openServo == 1){
    // Move o servo2 para a posição 180 graus por 2 segundos e, em seguida, retorna para 0 graus
    servo2.write(180);
    delay(2000);
    servo2.write(0);
  }
  else if(openServo == 2){
    // Move o servo3 para a posição 180 graus por 2 segundos e, em seguida, retorna para 0 graus
    servo3.write(180);
    delay(2000);
    servo3.write(0);
  }
}

void setup() {
  randomSeed(micros()); // Inicializa o gerador de números aleatórios com uma semente baseada no tempo atual (microssegundos)
  Serial.begin(115200); // Inicializa a comunicação serial com uma taxa de baud de 115200
  setup_wifi(); // Chama a função para configurar a conexão Wi-Fi
  client.setServer(mqtt_server, 1883); // Configura o servidor MQTT e a porta
  client.setCallback(callback); // Configura a função de callback para lidar com mensagens MQTT recebidas

  // Configuração dos servo motores
  servo1.attach(servoPin1, 500, 2400);
  servo2.attach(servoPin2, 500, 2400);
  servo3.attach(servoPin3, 500, 2400);

  // Define a posição inicial dos servo motores como 0 graus
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}

void loop() {
  if (!client.connected()) {
    reconnect(); // Se a conexão MQTT não estiver ativa, tenta reconectar
  }
  client.loop(); // Mantém a conexão MQTT ativa e lida com mensagens MQTT recebidas
}