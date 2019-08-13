#include "DHT_11.h"

DHT dht_1(DHTPIN_1,DHTTYPE);
DHT dht_2(DHTPIN_2,DHTTYPE);
DHT dht_3(DHTPIN_3,DHTTYPE);

//Funcao para leitura do DHT11
void lerDHT11(int *umi, int *tem, int id)
{
  if(id == 1){
    *umi = dht_1.readHumidity();
    *tem = dht_1.readTemperature();
  }else if(id == 2){
    *umi = dht_2.readHumidity();
    *tem = dht_2.readTemperature();
  }else if(id == 3){
    *umi = dht_3.readHumidity();
    *tem = dht_3.readTemperature();
  }

  if(isnan(*tem) || isnan(*umi)){
    #ifdef DEBUG_SERIAL
    Serial.println("Falha na leitura do DHT11...");
    #endif
  }else{
    #ifdef DEBUG_SERIAL
    Serial.print("Umidade: ");
    Serial.println(*umi);
    Serial.print("Temperatura: ");
    Serial.println(*tem);
    #endif
  }
}

void inicializaDHT11(void)
{
    dht_1.begin();
    dht_2.begin();
    dht_3.begin();
}
