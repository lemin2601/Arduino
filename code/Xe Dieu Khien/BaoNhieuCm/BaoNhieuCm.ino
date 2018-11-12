#define MAX_SPEED 255 //từ 0-255
#define Min_SPEED 0
//Motor A
int enA = 8;
int in1 = 7;
int in2 = 6; 
 
//Motor B
int in3 = 5; 
int in4 = 4; 
int enB = 3;  

//set bluetooth
char c;
int led = 13;


void Motor(char n,int speeds,int cm){
  int high = 255-speeds;
  switch(n){
    case '1': //tien
        //Running motor A
        digitalWrite(in1, HIGH);           digitalWrite(in4, high);
        digitalWrite(in2, 255-speeds);          digitalWrite(in3, 255-speeds);
        analogWrite(enA, 255);           analogWrite(enB, 255);  
    break;
    case '2': //lui
        //Running motor A
        digitalWrite(in1, LOW);           digitalWrite(in4, LOW);
        digitalWrite(in2, speeds);           digitalWrite(in3, speeds);
        analogWrite(enA, 255);          analogWrite(enB, 255);  
    break;
    case '3': //trai
        //Running motor A
        digitalWrite(in1, LOW);          digitalWrite(in4, HIGH);
        digitalWrite(in2, HIGH);           digitalWrite(in3, LOW);
        analogWrite(enA, 255);         analogWrite(enB, 255);  
    break;
    case '4': //lui
        //Running motor A
        digitalWrite(in1, HIGH); 
        digitalWrite(in4, LOW);
        digitalWrite(in2, LOW); 
        digitalWrite(in3, HIGH);
        analogWrite(enA, 255);   
        analogWrite(enB, 255);  
    break;
    case '6': //lui
        default:
        //Running motor A
        digitalWrite(in1, LOW); 
        digitalWrite(in4, LOW);
        digitalWrite(in2, LOW);  
        digitalWrite(in3, LOW);
        analogWrite(enA, 255);  
        analogWrite(enB, 255);  
    break;
     
  }
  delay(cm);
   //Running motor A
  digitalWrite(in1, LOW);        digitalWrite(in4, LOW);
  digitalWrite(in2, LOW);         digitalWrite(in3, LOW);
  analogWrite(enA, 255);         analogWrite(enB, 255); 
 }

void setup()
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
 
void motor_1_Dung() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
 
void motor_2_Dung() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
 
void motor_1_Tien(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, Min_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(in1, HIGH);// chân này không có PWM
  analogWrite(in2, 255 - speed);
}
 
void motor_1_Lui(int speed) {
  speed = constrain(speed, Min_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(in1, LOW);// chân này không có PWM
  analogWrite(in2, speed);
}
 
void motor_2_Tien(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, Min_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  analogWrite(in3, speed);
  digitalWrite(in4, LOW);// chân này không có PWM
}
 
void motor_2_Lui(int speed) {
  speed = constrain(speed, Min_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  analogWrite(in4, 255 - speed);
  digitalWrite(in3, HIGH);// chân này không có PWM
}
 
void loop()
{
  Motor('1',100,300);
  delay(5000);//tiến 5 s
  
}
