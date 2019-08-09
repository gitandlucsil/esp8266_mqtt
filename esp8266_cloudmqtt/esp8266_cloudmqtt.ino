#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DEBUG
#define MAX_TENTATIVAS_CONEXAO 10
#define INTERVALO_ENVIO 5000
#define DHTPIN D4
#define DHTTYPE DHT11
#define ONE_WIRE_BUS D4

DHT dht(DHTPIN,DHTTYPE);
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
//Definição dos periféricos
const byte LED_1 = 16;  //Led do Acionamento pelo botao
const byte LED_2 = 2; //Led do Alarme do DHT11
//Definição da rede WiFi
const char* ssid = ";
const char* senha = "";
//Definição do MQTT
const char* mqttServer = "";
const char* mqttUser = "";
const char* mqttPassword = "";
const char* mqttTopicSub1 = "";
//const char* mqttTopicSub2 = "";
const char* mqttTopicUmidade = "";
const char* mqttTopicAlarmeUmidade = "";
const char* mqttTopicTemperatura = "";
const int mqttPort = 0;

int ultimoEnvioMQTT = 0;
int alarmeUmidade = 0;
int umidade = 0;
int temperatura = 0;

WiFiClient espClient;
PubSubClient client(espClient);

DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

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

 
void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void lerSensorTemperatura(void)
{
   sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);
  Serial.print("Temp C: ");
  Serial.print(tempC);
}
void setup()
{
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, LOW);
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
  //client.subscribe(mqttTopicSub1,1);
  //client.subscribe(mqttTopicSub2);
  //client.subscribe(mqttTopicAlarmeUmidade,1);
   sensors.begin();
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  Serial.println();
  //dht.begin();
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
//  if (String(topic) == mqttTopicSub1) {
//    if (strMSG == "1") {        //se msg "1"
//      digitalWrite(LED_1, LOW);  //coloca saída em LOW para ligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
//    } else if (strMSG == "0") {  //se msg "0"
//      digitalWrite(LED_1, HIGH);   //coloca saída em HIGH para desligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
//    }
//  }
//  } else if (String(topic) == "board/led2") {
//    if (strMSG == "1") {        //se msg "1"
//      digitalWrite(LED_2, LOW);  //coloca saída em LOW para ligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
//    } else if (strMSG == "0") {  //se msg "0"
//      digitalWrite(LED_2, HIGH);   //coloca saída em HIGH para desligar a Lampada - > o módulo RELE usado tem acionamento invertido. Se necessário ajuste para o seu modulo
//    }
//  }
// if (String(topic) == mqttTopicAlarmeUmidade) {
//  alarmeUmidade = strMSG.toInt();
// }
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
      //client.subscribe(mqttTopicSub1, 1); //nivel de qualidade: QoS 1
      //client.subscribe(mqttTopicSub2, 2); //nivel de qualidade: QoS 2
      //client.subscribe(mqttTopicAlarmeUmidade,1);
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

  //envia a cada tempo
  if((millis() - ultimoEnvioMQTT) > INTERVALO_ENVIO){
    lerSensorTemperatura();
//    enviaDHT();
    ultimoEnvioMQTT = millis();
//    verificaAlarmeDHT11();
  }
  
  client.loop();
}

////Funcao para leitura do DHT11
//void enviaDHT(){
//
//  char MsgUmidadeMQTT[10];
//  char MsgTemperaturaMQTT[10];
//  
//  umidade = dht.readHumidity();
//  temperatura = dht.readTemperature();
//
//  if(isnan(temperatura) || isnan(umidade)){
//    #ifdef DEBUG
//    Serial.println("Falha na leitura do DHT11...");
//    #endif
//  }else{
//    #ifdef DEBUG
//    Serial.print("Umidade: ");
//    Serial.println(umidade);
//    Serial.print("Temperatura: ");
//    Serial.println(temperatura);
//    #endif
//
//    sprintf(MsgUmidadeMQTT,"%d",umidade);
//    client.publish(mqttTopicUmidade,MsgUmidadeMQTT);
//    sprintf(MsgTemperaturaMQTT,"%d",temperatura);
//    client.publish(mqttTopicTemperatura,MsgTemperaturaMQTT);
//  }
//}
//
//void verificaAlarmeDHT11 (void){
//   if(umidade >= alarmeUmidade){
//    Serial.println("Alarme de umidade alta!!!!!!!!!!");
//    Serial.print(umidade);
//    Serial.print(" , ");
//    Serial.print(alarmeUmidade);
//    digitalWrite(LED_1, LOW);
//   }else{
//    digitalWrite(LED_1, HIGH);
//   }
//}
