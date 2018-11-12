#define TRIG_PIN 12
#define ECHO_PINTruoc 2
#define ECHO_PINSau 8
#define ECHO_PINTrai 4
#define ECHO_PINPhai 7
#define TIME_OUT 5000

float GetDistance(int pin)
{
  long duration, distanceCm;
   
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
 
  duration = pulseIn(pin, HIGH, TIME_OUT);
 
  // convert to distance
  distanceCm = duration / 29.1 / 2;
  
  return distanceCm;
}
 
void setup() {  
  Serial.begin(9600);
 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PINTruoc, INPUT);
    pinMode(ECHO_PINSau, INPUT);
      pinMode(ECHO_PINTrai, INPUT);
        pinMode(ECHO_PINPhai, INPUT);
}
 
void loop() {
  long distanceTruoc = GetDistance(ECHO_PINTruoc);
 long distanceSau = GetDistance(ECHO_PINSau);
 long distanceTrai = GetDistance(ECHO_PINTrai);
 long distancePhai = GetDistance(ECHO_PINPhai);
    
    Serial.print("Trước : ");
    Serial.print(distanceTruoc);
    Serial.print(" Sau : ");
    Serial.print(distanceSau);
    Serial.print(" Trái : ");
    Serial.print(distanceTrai);
    Serial.print(" Phai : ");
    Serial.print(distancePhai);
    Serial.println("");
  
  delay(500);
}
