#include <Servo.h> 
 
#define SERVO_PIN 9
  int i = 0;
  int delta = 1;
Servo gServo;
 int k;
void setup() 
{ 
  pinMode(5,OUTPUT);
   pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);

    gServo.attach(SERVO_PIN); 
    Serial.begin(9600);
}
 
void loop() 
{ 
      
      k = random(0,8);
      k = k%8;
      Serial.println(k);
        switch(k){
          case 0:

           digitalWrite(5,HIGH);
           break;
        case 1:
          digitalWrite(5,LOW);
           case 2:
           digitalWrite(6,HIGH);
           break;
        case 3:
          digitalWrite(6,LOW);
           case 4:
           digitalWrite(7,HIGH);
           break;
        case 5:
          digitalWrite(7,LOW);
        
      }
      i+=delta;
      gServo.write(i);
      delay(1000);
      if(i ==180 || i ==0){
        delta = - delta;
    }

}
