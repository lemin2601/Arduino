#include <dht.h>

dht DHT;

#define DHT11_PIN 7
#define Nguon 4
void setup(){
  Serial.begin(9600);
  pinMode(Nguon,OUTPUT);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  if(DHT.temperature > 20){
    digitalWrite(Nguon,HIGH);
    }else{
        digitalWrite(Nguon,LOW);
      }
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
//  Serial.println(chk);
//  if(DHT.temperature > 20){
//    Serial.print("Chinh xacs con me no");}
//  Serial.print("Humidity = ");
//  Serial.println(DHT.humidity);
//  
//  //delay(5000);
//  //digitalWrite(Nguon,LOW);
  delay(1000);
}

