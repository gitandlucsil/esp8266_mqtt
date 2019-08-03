const byte LED_1 = 2; //Pino do LED do NodeMCU
const byte LED_2 = 16; //Pino do LED do NodeMCU
const byte LED_EXT_1 = D1; //Pino de LED externo na saida digital
void setup() 
{
  pinMode(LED_1,OUTPUT);
  pinMode(LED_2,OUTPUT);
  pinMode(LED_EXT_1,OUTPUT);
}

void loop() 
{
  digitalWrite(LED_1,LOW);//Acender o LED
  digitalWrite(LED_2,HIGH);//Apagar o LED
  digitalWrite(LED_EXT_1,HIGH);//Acender o LED
  delay(1000);
  digitalWrite(LED_1,HIGH);//Apagar o LED
  digitalWrite(LED_2,LOW);//Acender o LED
  digitalWrite(LED_EXT_1,LOW);//Acender o LED
  delay(1000);
}
