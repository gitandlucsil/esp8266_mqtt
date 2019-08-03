#include <ESP8266WiFi.h>

#define MAX_TENTATIVAS_CONEXAO 10

const char* ssid = "";
const char* senha = "";
const byte LED = 2;

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
  Serial.begin(115200);
  delay(10);
  if(conectaWiFi() == true){
    server.begin();
    Serial.println("Servidor inicializado");
  }
}
int cont = 0;
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
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta http-equiv='refresh' content='2'>");
  client.println("<h1>Curso controle dispositivos remotamente com ESP8266</h1>");
  client.println("<h2>Ola Makers!</h2>");
  client.println(cont);
  client.println("</html>"); 
  cont++;
}
