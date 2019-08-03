const byte LED_1 = 2; //Pino do LED do NodeMCU
const byte LED_2 = 16; //Pino do LED do NodeMCU
void setup() 
{
  pinMode(LED_1,OUTPUT);
  pinMode(LED_2,OUTPUT);
}

void loop() 
{
  digitalWrite(LED_1,LOW);//Acender o LED
  digitalWrite(LED_2,HIGH);//Acender o LED
  delay(1000);
  digitalWrite(LED_1,HIGH);//Apagar o LED
  digitalWrite(LED_2,LOW);//Acender o LED
  delay(1000);
}
