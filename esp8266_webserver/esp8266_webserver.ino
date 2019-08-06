#include <ESP8266WiFi.h>

#define MAX_TENTATIVAS_CONEXAO 10

const char* ssid = "";
const char* senha = "";
const byte LED_1 = 2;
const byte LED_2 = 16;

WiFiServer server(80);

bool conectaWiFi(void)
{
  Serial.print("Conectando a rede ");
  Serial.println(ssid);

  WiFi.begin(ssid,senha);
  int tentativas = 0;
  while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
      tentativas++;
      if(tentativas == MAX_TENTATIVAS_CONEXAO){
        return false;
      }
  }
  Serial.print("Conectado a rede ");
  Serial.print(ssid);
  Serial.println("!");
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());
  return true;
}
void setup() 
{
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  Serial.begin(115200);
  delay(10);
  if(conectaWiFi() == true){
    server.begin();
    Serial.println("Servidor inicializado");
  }
}
boolean statusLED_1 = LOW;
boolean statusLED_2 = LOW;
void loop() 
{
  WiFiClient client = server.available();
  if(!client){
    return;
  }
  while(!client.available()){
    delay(1);
  }
  Serial.println("Cliente conectado!");

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if(req.indexOf("LED1on") != -1){
    digitalWrite(LED_1,LOW);
    statusLED_1 = HIGH;
  }else if(req.indexOf("LED1off") != -1){
    digitalWrite(LED_1,HIGH);
    statusLED_1 = LOW;
  }
  if(req.indexOf("LED2on") != -1){
    digitalWrite(LED_2,LOW);
    statusLED_2 = HIGH;
  }else if(req.indexOf("LED2off") != -1){
    digitalWrite(LED_2,HIGH);
    statusLED_2 = LOW;
  }

  if(req.indexOf("LEDTodoson") != -1){
    digitalWrite(LED_1,LOW);
    digitalWrite(LED_2,LOW);
    statusLED_1 = HIGH;
    statusLED_2 = HIGH;
  }else if(req.indexOf("LEDTodosoff") != -1){
    digitalWrite(LED_1,HIGH);
    digitalWrite(LED_2,HIGH);
    statusLED_1 = LOW;
    statusLED_2 = LOW;
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  //client.println("<meta http-equiv='refresh' content='2'>");
  client.println("<h1>Curso controle dispositivos remotamente com ESP8266</h1>");
  client.println("<h2>Ola Makers!</h2>");
  if(!statusLED_1){
    client.println("<p>LED 1 DESLIGADO <a href=\"LED1on\"><button>LIGAR</button></a></p>"); 
  }else{
    client.println("<p>LED 1 LIGADO <a href=\"LED1off\"><button>DESLIGAR</button></a></p>"); 
  }
  if(!statusLED_2){
    client.println("<p>LED 2 DESLIGADO <a href=\"LED2on\"><button>LIGAR</button></a></p>"); 
  }else{
    client.println("<p>LED 2 LIGADO <a href=\"LED2off\"><button>DESLIGAR</button></a></p>"); 
  }
  client.println("<p><a href=\"LEDTodoson\"><button>LIGAR TODOS</button></a></p>"); 
  client.println("<p><a href=\"LEDTodosoff\"><button>DESLIGAR TODOS</button></a></p>"); 

  client.println("</html>"); 
}
