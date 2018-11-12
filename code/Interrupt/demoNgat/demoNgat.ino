volatile int count = 0;
void DaoLed(){
  if(count ==20000){
    count = 0;
    digitalWrite(13,HIGH);
    Serial.println("Interrupt");
  }
  if(count == 1000){
    digitalWrite(13,LOW);
  }
  count++;
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(0,DaoLed,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
 Serial.println("loop print");
  delay(1000);

}
