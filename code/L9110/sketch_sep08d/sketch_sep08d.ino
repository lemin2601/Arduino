const int traiam = 12;
const int traiduong = 11;
const int phaiam = 7;
const int phaiduong = 6;


void setup() {
  // put your setup code here, to run once:
pinMode(traiam, OUTPUT);
pinMode(phaiam, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite(traiam,HIGH);
 digitalWrite(phaiam,HIGH);
 analogWrite(traiduong,0);
 analogWrite(phaiduong,0);
 delay(5000);
 digitalWrite(traiam,LOW);
 digitalWrite(phaiam,LOW);
 analogWrite(traiduong,200);
 analogWrite(phaiduong,200);
  delay(1000);
}

