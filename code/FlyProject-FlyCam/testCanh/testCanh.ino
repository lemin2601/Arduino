const int CANHL1 = 3;
const int CANHL2 = 5;
const int CANHR1 = 6;
const int CANHR2 = 9;


void setup() {
  // put your setup code here, to run once:
  pinMode(CANHL1,OUTPUT);
  pinMode(CANHL2,OUTPUT);
  pinMode(CANHR1,OUTPUT);
  pinMode(CANHR2,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  int i  =0;
//  for(i =10;i<250;i+=1){
//    Serial.println(i);
//      analogWrite(CANHL1,i);
//  analogWrite(CANHL2,i);
//  analogWrite(CANHR1,i);
//  analogWrite(CANHR2,i);
//  delay(200);
  analogWrite(CANHL1,255);
  analogWrite(CANHL2,255);
  analogWrite(CANHR1,255);
  analogWrite(CANHR2,255);
  delay(5000);
    analogWrite(CANHL1,0);
  analogWrite(CANHL2,0);
  analogWrite(CANHR1,0);
  analogWrite(CANHR2,0);
  delay(5000);
  }

  

