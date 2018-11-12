#include <Servo.h> 
// cài đặt chân cho cảm biến siêu âm
#define TRIG_PIN 13
#define ECHO_PIN 12
#define TIME_OUT 5000

 //cài đặt chân cho servo
#define SERVO_PIN 9

//cài đặt chân cho motor
//Motor A
int enA = 8;
int in1 = 7;
int in2 = 6; 
 
//Motor B
int in3 = 5; 
int in4 = 4; 
int enB = 3;  
int speeds = 100;
//set bluetooth
char c;

Servo gServo;

//Hàm trả Khoảng cách - Cảm biến siêu âm
float GetDistance()
{
  long duration, distanceCm;
   
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH, TIME_OUT);
 
  // convert to distance
  distanceCm = duration / 29.1 / 2;
  
  return distanceCm;
}
void Motor(char n){
  //Đọc kí tự và điều khiển motor tương ứng
  // 1 lùi
  // 2 tiến
  // 3 quay trái
  // 4 quay phải
  // 5 tăng tốc
  // 6 giảm tốc
  //
  int speeds =200;
  switch(n){
    case '1': //lui
        //Running motor A
        digitalWrite(in1, HIGH);        digitalWrite(in4, HIGH);
        digitalWrite(in2, LOW);          digitalWrite(in3, LOW);
        analogWrite(enA, speeds);   
        analogWrite(enB, speeds);  
    break;
    case '2': //lui
        //Running motor A
        digitalWrite(in1, LOW);          digitalWrite(in4, LOW);
        digitalWrite(in2, HIGH);           digitalWrite(in3, HIGH);
        analogWrite(enA, speeds);   
        analogWrite(enB, speeds);  
    break;
    case '3': //trai
        //Running motor A
        digitalWrite(in1, LOW);          digitalWrite(in4, HIGH);
        digitalWrite(in2, HIGH);           digitalWrite(in3, LOW);
        analogWrite(enA, speeds); 
        analogWrite(enB, speeds);  
    break;
    case '4': //lui
        //Running motor A
        digitalWrite(in1, HIGH);        digitalWrite(in4, LOW);
        digitalWrite(in2, LOW);         digitalWrite(in3, HIGH);
        analogWrite(enA, speeds);   
        analogWrite(enB, speeds);  
    break;
    case '6': //lui
    default:
        //Running motor A
        digitalWrite(in1, LOW);         digitalWrite(in4, LOW);
        digitalWrite(in2, LOW);          digitalWrite(in3, LOW);
        analogWrite(enA, speeds);  
        analogWrite(enB, speeds);  
    break;
    
  }
 }
 
void setup() {  
  Serial.begin(9600);
 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  gServo.attach(SERVO_PIN); 
  
  // set motor
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT); 
  pinMode(enB,OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
void Xoay(bool isTrai,int speeds,int Angle){
    //góc tính ta thời gian delay
    // còn dựa trên tốc độ speed
    if(isTrai){
        //Running motor A
        digitalWrite(in1, LOW);          digitalWrite(in4, HIGH);
        digitalWrite(in2, HIGH);           digitalWrite(in3, LOW);
        analogWrite(enA, speeds); 
        analogWrite(enB, speeds);  
    }else{
        //Running motor A
        digitalWrite(in1, LOW);          digitalWrite(in4, HIGH);
        digitalWrite(in2, HIGH);          digitalWrite(in3, LOW);
        analogWrite(enA, speeds); 
        analogWrite(enB, speeds);  
    }
    delay(Angle);
             //Running motor A
        digitalWrite(in1, LOW);         digitalWrite(in4, LOW);
        digitalWrite(in2, LOW);          digitalWrite(in3, LOW);
        analogWrite(enA, speeds);  
        analogWrite(enB, speeds);  
}
void demoTwo() {
 // this function will run the motors across the range of possible speeds
 // note that maximum speed is determined by the motor itself and the operating voltage
 // the PWM values sent by analogWrite() are fractions of the maximum speed possible
 // by your hardware
 // turn on motors
digitalWrite(in1, LOW);          digitalWrite(in4, HIGH);
        digitalWrite(in2, HIGH);           digitalWrite(in3, LOW);
 // accelerate from zero to maximum speed
 for (int i = 0; i < 256; i++)
 {
 analogWrite(enA, i);
 analogWrite(enB, i);
 delay(20);
 }
 // decelerate from maximum speed to zero
 for (int i = 255; i >= 0; --i)
 {
 analogWrite(enA, i);
 analogWrite(enB, i);
 delay(20);
 }
 // now turn off motors
 digitalWrite(in1, LOW);
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW);
}

void loop() {
//  Motor('1');delay(100);
//  Motor('2');delay(200);
//  Motor('3');delay(500);
//  Motor('4');delay(1000);
//  Motor('5');delay(3000);
//  
//  Xoay(true,500,100);
demoTwo();
  ;delay(3000);
//  long distance = GetDistance();
//  if( distance <=0 || distance >7){
//    Motor('1');
//  }else{
//    Motor('2');    
//  }
//int i =0;
//  for(i=0;i<10;i++){
//    Xoay(true,10*i,100*i);
//    delay(2000);
//  }

//  while(Serial.available()){//kiem tra byte de doc
//    delay(30);// để ổn định hơn
//    
//    c = Serial.read();
//
//    Serial.println(c);
//    Motor(c);
//  }
//  long distance = GetDistance();
//  
// 
//  if (distance <= 0)
//  {
//    Serial.println("Echo time out !!");
//  }
//  else
//  {   
//    Serial.print("Distance to nearest obstacle (cm): ");
//    Serial.println(distance);
//  }
//  delay(500);
//
//   gServo.write(0);
//    delay(1000);
//  
//    gServo.write(90);
//    delay(1000);
//  
//    gServo.write(180);
//    delay(1000);
//     gServo.write(90);
//    delay(1000);
}
