#include "WiFi_Conn.h"

//Definição da rede WiFi
const char* ssid = "";
const char* senha = "";

bool conectaWiFi(void)
{
#ifdef DEBUG_SERIAL
  Serial.print("Conectando a rede ");
  Serial.println(ssid);
#endif

  WiFi.begin(ssid, senha);
  uint8_t tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG_SERIAL
    Serial.print(".");
#endif
    tentativas++;
    if (tentativas == MAX_TENTATIVAS_CONEXAO) {
      return false;
    }
  }
#ifdef DEBUG_SERIAL
  Serial.print("Conectado a rede ");
  Serial.print(ssid);
  Serial.println("!");
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());
#endif
  return true;
}
