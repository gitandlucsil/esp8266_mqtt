#include <ESP8266WiFi.h>

const char* ssid = "";
const char* senha = "";

void setup() 
{
  Serial.begin(115200);
  delay(10);
  Serial.print("Conectando a rede ");
  Serial.println(ssid);

  WiFi.begin(ssid,senha);
  while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
  }
  Serial.print("Conectado a rede ");
  Serial.print(ssid);
  Serial.println("!");
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{

}
