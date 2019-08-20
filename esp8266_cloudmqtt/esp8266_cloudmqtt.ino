#include "DHT_11.h"
#include "WiFi_Conn.h"
#include "MQTT_PubSub.h"
#include <PubSubClient.h>

const char* mqttServer = "";
const char* mqttUser = "";
const char* mqttPassword = "";
const int mqttPort = 0;

int umidade = 0; //Variavel utilizada para armazenar o valor da umidade
int temperatura = 0;//Variavel utilizada para armazenar o valor da temperatura

int ultimoEnvioMQTT = 0; //Variavel que armazena o timestamp do ultimo envio de leitura para o broker MQTT

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) 
{
  //Nao sendo utilizada
}

void conecta_broket_mqtt(void)
{
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

void enviatDHT11MQTT(int valor_umidade,int valor_temperatura,const char *topico)
{
    //Monta o json
    String json = "{";
    json += "\"umidade\":";
    json += valor_umidade;
    json += ",\"temperatura\":";
    json += valor_temperatura;
    json += "}";
    //Publica no topico
    client.publish(topico,json.c_str());
}
void setup()
{
  Serial.begin(115200);
  conectaWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  conecta_broket_mqtt();
  inicializaDHT11();
}
void loop()
{
  if (!client.connected()) {
    reconect();
  }
  if((millis() - ultimoEnvioMQTT) > INTERVALO_ENVIO){
    lerDHT11(&umidade,&temperatura,1);
    enviatDHT11MQTT(umidade,temperatura,TOPICO_DHT11_1);
    lerDHT11(&umidade,&temperatura,2);
    enviatDHT11MQTT(umidade,temperatura,TOPICO_DHT11_2);
    lerDHT11(&umidade,&temperatura,3);
    enviatDHT11MQTT(umidade,temperatura,TOPICO_DHT11_3);
    ultimoEnvioMQTT = millis();
  }
  client.loop();
}
