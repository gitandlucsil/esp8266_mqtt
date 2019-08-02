const byte LED = 2; //Pino do LED do NodeMCU

void setup() 
{
  pinMode(LED,OUTPUT);
}

void loop() 
{
  digitalWrite(LED,LOW);//Acender o LED
  delay(1000);
  digitalWrite(LED,HIGH);//Apagar o LED
  delay(1000);
}
