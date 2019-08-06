#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DEBUG
#define MAX_TENTATIVAS_CONEXAO 10
//Definição dos periféricos
const byte LED_1 = 2;
const byte LED_2 = 16;
//Definição da rede WiFi
const char* ssid = "";
const char* senha = "";
//Definição do MQTT
const char* mqttServer = "";
const char* mqttUser = "";
const char* mqttPassword = "";
const char* mqttTopicSub1 = "";
const char* mqttTopicSub2 = "";
const int mqttPort = ;

WiFiClient espClient;
PubSubClient client(espClient);

bool conectaWiFi(void)
{
#ifdef DEBUG
  Serial.print("Conectando a rede ");
  Serial.println(ssid);
#endif

  WiFi.begin(ssid, senha);
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG
    Serial.print(".");
#endif
    tentativas++;
    if (tentativas == MAX_TENTATIVAS_CONEXAO) {
      return false;
    }
  }
#ifdef DEBUG
  Serial.print("Conectado a rede ");
  Serial.print(ssid);
  Serial.println("!");
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());
#endif
  return true;
}

void setup()
{
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  Serial.begin(115200);
  conectaWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
#ifdef DEBUG
    Serial.print("Conectando ao Broker MQTT....");
#endif
    if (client.connect("cursoESP8266MQTT", mqttUser, mqttPassword)) {
#ifdef DEBUG
      Serial.print("Conectado");
#endif
    } else {
#ifdef DEBUG
      Serial.print("falha estado ");
      Serial.print(client.state());
#endif
      delay(2000);
    }
  }
  //subscreve nos tópicos
  client.subscribe(mqttTopicSub1);
  client.subscribe(mqttTopicSub2);
}
void callback(char* topic, byte* payload, unsigned int length) {

  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

#ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
#endif

  //aciona saída conforme msg recebida
  if (String(topic) == "board/led1") {
    if (strMSG == "1") {        //se msg "1"
      digitalWrite(LED_1, LOW);  //coloca saída em LOW para ligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
    } else if (strMSG == "0") {  //se msg "0"
      digitalWrite(LED_1, HIGH);   //coloca saída em HIGH para desligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
    }
  } else if (String(topic) == "board/led2") {
    if (strMSG == "1") {        //se msg "1"
      digitalWrite(LED_2, LOW);  //coloca saída em LOW para ligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
    } else if (strMSG == "0") {  //se msg "0"
      digitalWrite(LED_2, HIGH);   //coloca saída em HIGH para desligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
    }
  }
}

//função pra reconectar ao servido MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {
#ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
#endif

    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("cursoESP8266MQTT", mqttUser, mqttPassword) :
                     client.connect("cursoESP8266MQTT");

    if (conectado) {
#ifdef DEBUG
      Serial.println("Conectado!");
#endif
      //subscreve no tópico
      client.subscribe(mqttTopicSub1, 1); //nivel de qualidade: QoS 1
      client.subscribe(mqttTopicSub2, 2); //nivel de qualidade: QoS 2
    } else {
#ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
#endif
      //Aguarda 10 segundos
      delay(10000);
    }
  }
}

void loop()
{
  if (!client.connected()) {
    reconect();
  }
  client.loop();
}
