#include "Timer.h"
#include "kIndustryCalendar.h"
#include "WorkScheduler.h"
#include <Servo.h> 
//trai  5 40  70
//phai  45  70  105
// khai báo biến sử dụng vào các chân ngõ ra v.v...
//Bánh Xe
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0

#define Encoder_Trai 18
#define Encoder_Phai 19

#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
//Cảm biến siêu âm
#define Truoc_Echo 22
#define Truoc_Trig 23

#define Sau_Echo_1 24
#define Sau_Trig_1 25
#define Sau_Echo_2 26
#define Sau_Trig_2 27

#define Trai_Echo_1 28
#define Trai_Trig_1 29
#define Trai_Echo_2 30
#define Trai_Trig_2 31

#define Phai_Echo_1 32
#define Phai_Trig_1 33
#define Phai_Echo_2 34
#define Phai_Trig_2 35

#define AmThanh 11

#define Led_Do_1 53
#define Led_Do_2 52
#define Led_Vang_Trai_1  46
#define Led_Vang_Trai_2  51
#define Led_Vang_Phai_1  48
#define Led_Vang_Phai_2  50
#define Led_Trang_1 47
#define Led_Trang_2 49
// servo 
#define ServoTruoc_Trai 8
#define ServoTruoc_Phai 9
Servo servo_trai; // điều khiển bánh trước trái
Servo servo_phai; // điều khiển bánh trước phải

// Motor
bool motor_isUse = false;   // true là kích hoạt sử dụng // false là dừng
bool motor_isChange = false;// có thay đổi để cập nhật
int motor_vanToc_trai = 0;  // tốc độ điện motor 0 -255
int motor_vanToc_phai = 0;
int motor_servo = 0;    // 0 là thẳng <0 trái > 0 phải
bool motor_isTien = true;   // true đi thẳng  false đi lùi
//encoder 
volatile long motor_encoder_trai = 0;     // lưu giá trị encoder trai
volatile long motor_encoder_phai = 0;     // lưu giá trị encoder phải
unsigned long motor_encoder_timeDelay_trai = 0; // thời gian delay để tránh nhiễu
unsigned long motor_encoder_timeDelay_phai = 0;
bool A_set = true;                // biến hỗ trợ đếm encoder
bool B_set = true;
// led
bool led_isUse = false;     // turn on or turn off
bool led_isHigh = false;    // Trạng thái led hiện tại
bool led_isChangeType = false;  // có thay đổi để cập nhật
int led_type;           //loại nhấp nháy switch case;
// am thanh
bool amThanh_isChange = false;  // cập nhật thay đổi trạng thái
bool amThanh_isUse = false;   // turn on or off
bool amThanh_isHigh = false;  // trạng thái hiện tại của âm thanh là HIGH ??
int amThanh_type;       // loại âm thanh sẽ phát switch case;
int amThanh_count = 0;      // lưu trữ số lần tiếng kêu phát ra
//cảm biến siêu âm
unsigned long distance_timeOut = 5000;  // tối đa tính 100 cm trở lại
unsigned long distance_get_time = 0;
///////////////////////các workscheduler
WorkScheduler *amThanh_WorkScheduler;      //Tiến trình phát âm thanh
WorkScheduler *led_WorkScheduler;        //Tiến trình phát led
WorkScheduler *motor_WorkScheduler;         //Tiến trình motor chạy
WorkScheduler *bluetooth_WorkScheduler;   // tiến trình đọc cảm biến âm thanh
WorkScheduler *motor_controlTraight_WorkScheduler;  //tiến trình điều khiển xe 
WorkScheduler *motor_Check_WorkScheduler; // sử dụng để kiểm tra giá trị


float distance_get(int vitri){ // 1 trước ; 2 sau1 ; 3 trai1; 4 phai1; 5 sau2; 6 trai2; 7 phai2
  switch(vitri){
    case 1: // trước
      digitalWrite(Truoc_Trig,LOW);
      delayMicroseconds(2);
      digitalWrite(Truoc_Trig,HIGH);
      delayMicroseconds(5);
      digitalWrite(Truoc_Trig,LOW);
      distance_get_time = pulseIn(Truoc_Echo,HIGH,distance_timeOut);
      return int(distance_get_time/2/29.412);
    case 2: // sau 
      digitalWrite(Sau_Trig_1,LOW);
      delayMicroseconds(2);
      digitalWrite(Sau_Trig_1,HIGH);
      delayMicroseconds(5);
      digitalWrite(Sau_Trig_1,LOW);
      distance_get_time = pulseIn(Sau_Echo_1,HIGH,distance_timeOut);
      return float(distance_get_time/2/29.412);
    case 3: // trai 1
      digitalWrite(Trai_Trig_1,LOW);
      delayMicroseconds(2);
      digitalWrite(Trai_Trig_1,HIGH);
      delayMicroseconds(5);
      digitalWrite(Trai_Trig_1,LOW);
      distance_get_time = pulseIn(Trai_Echo_1,HIGH,distance_timeOut);
      return float(distance_get_time/2/29.412);
    case 4: // pahi 1
      digitalWrite(Phai_Trig_1,LOW);
      delayMicroseconds(2);
      digitalWrite(Phai_Trig_1,HIGH);
      delayMicroseconds(5);
      digitalWrite(Phai_Trig_1,LOW);
      distance_get_time = pulseIn(Phai_Trig_1,HIGH,distance_timeOut);
      return float(distance_get_time/2/29.412);
    case 5: // sau 2
      digitalWrite(Sau_Trig_2,LOW);
      delayMicroseconds(2);
      digitalWrite(Sau_Trig_2,HIGH);
      delayMicroseconds(5);
      digitalWrite(Sau_Trig_2,LOW);
      distance_get_time = pulseIn(Sau_Echo_2,HIGH,distance_timeOut);
      return float(distance_get_time/2/29.412);
    case 6: // trai 2
      digitalWrite(Trai_Trig_2,LOW);
      delayMicroseconds(2);
      digitalWrite(Trai_Trig_2,HIGH);
      delayMicroseconds(5);
      digitalWrite(Trai_Trig_2,LOW);
      distance_get_time = pulseIn(Trai_Echo_2,HIGH,distance_timeOut);
      return int(distance_get_time/2/29.412);
    case 7: // phai 2
      digitalWrite(Phai_Trig_2,LOW);
      delayMicroseconds(2);
      digitalWrite(Phai_Trig_2,HIGH);
      delayMicroseconds(5);
      digitalWrite(Phai_Trig_2,LOW);
      distance_get_time = pulseIn(Phai_Trig_2,HIGH,distance_timeOut);
      return float(distance_get_time/2/29.412);
        
  }
} 
void amThanh_ProcessWork() { // 1. bíp bíp 2. bip bip 5 lần
  if(amThanh_isChange){   if(amThanh_isUse){amThanh_count =0; } else{digitalWrite(AmThanh,LOW);}    amThanh_isChange = false;
    // có thay đổi trạng thái âm thanh thì cập nhật
  }
  if(amThanh_isUse){
        amThanh_isHigh  = !amThanh_isHigh;  amThanh_count++;  digitalWrite(AmThanh,amThanh_isHigh);   //amThanh_timeDelay = millis();
    if(amThanh_count >= amThanh_type) amThanh_isUse = false;
  }
}
void led_ProcessWork(){ //1 trước 2 sau 3 trái nhấp nháy 4 phải nhấp nháy ; 5 nhấp nháy vàng
  if(led_isChangeType){
  led_isChangeType = false;
  digitalWrite(Led_Trang_1,LOW);
  digitalWrite(Led_Trang_2,LOW);
  digitalWrite(Led_Do_1,LOW);
  digitalWrite(Led_Do_2,LOW);
  digitalWrite(Led_Vang_Trai_1,LOW);
  digitalWrite(Led_Vang_Trai_2,LOW);
  digitalWrite(Led_Vang_Phai_1,LOW);
  digitalWrite(Led_Vang_Phai_2,LOW);
  }
  if(led_isUse){
  switch(led_type){
    case 1:// 2 led phía trước sáng ( tiến )
      digitalWrite(Led_Trang_1,HIGH);     digitalWrite(Led_Trang_2,HIGH);     break;
    case 2: // 2 đèn đỏ sáng
      digitalWrite(Led_Do_1,HIGH);      digitalWrite(Led_Do_2,HIGH);      break;
    case 3: // nhâp nháy vàng trái
        led_isHigh = !led_isHigh;
        digitalWrite(Led_Vang_Trai_1,led_isHigh); digitalWrite(Led_Vang_Trai_2,led_isHigh);     
      break;
    case 4: // nhâp nháy vàng phải
        led_isHigh = !led_isHigh; 
        digitalWrite(Led_Vang_Phai_1,led_isHigh);  digitalWrite(Led_Vang_Phai_2,led_isHigh);               
      break;
    case 5: // nhâp nháy vàng
        led_isHigh = !led_isHigh;
        digitalWrite(Led_Vang_Phai_1,led_isHigh);       digitalWrite(Led_Vang_Phai_2,led_isHigh); 
        digitalWrite(Led_Vang_Trai_1,led_isHigh);         digitalWrite(Led_Vang_Trai_2,led_isHigh);    
      break;  
    case 6: // nhâp tất cả
        led_isHigh = !led_isHigh;
        digitalWrite(Led_Vang_Phai_1,led_isHigh);       digitalWrite(Led_Vang_Phai_2,led_isHigh); 
        digitalWrite(Led_Vang_Trai_1,led_isHigh);         digitalWrite(Led_Vang_Trai_2,led_isHigh);  
        digitalWrite(Led_Trang_1,led_isHigh);       digitalWrite(Led_Trang_2,led_isHigh); 
        digitalWrite(Led_Do_1,led_isHigh);        digitalWrite(Led_Do_2,led_isHigh);
      break;        
    }//end switch
  }// end if isUse
}
void doEncoderTrai(){ // tiến encoder +++ ; lùi encoder --;
 if ( (unsigned long) (micros() - motor_encoder_timeDelay_trai) > 150  )
  {
    // thực hiện công việc sau delay
    if(digitalRead(Encoder_Trai) != A_set){    A_set = !A_set;  motor_encoder_trai +=motor_isTien?1:-1; }
    motor_encoder_timeDelay_trai = micros();
  }
}
void doEncoderPhai(){
if ( (unsigned long) (micros() - motor_encoder_timeDelay_phai) > 150  )
    // thực hiện công việc sau delay
    if(digitalRead(Encoder_Phai) != B_set){    B_set = !B_set;   motor_encoder_phai +=motor_isTien?1:-1;  }
    motor_encoder_timeDelay_phai = micros();
}

void motor_1_Dung() {   digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);}
void motor_2_Dung() {   digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);}
void motor_1_Tien(int speed) {  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  digitalWrite(IN1, HIGH);  analogWrite(IN2, 256 - speed);}
void motor_1_Lui(int speed) {  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  digitalWrite(IN1, LOW);  analogWrite(IN2, speed);}
void motor_2_Tien(int speed) {  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  analogWrite(IN3, speed);  digitalWrite(IN4, LOW);}
void motor_2_Lui(int speed) {  speed = constrain(speed, MIN_SPEED, MAX_SPEED);  analogWrite(IN3, 256 - speed);  digitalWrite(IN4, HIGH);}

void motor_ProcessWork(){ //motor_vanToc_trai ; motor_vanToc_phai ;  motor_isUse ; motor_isChange ;motor_servo
  if(motor_isChange){ // thực hiện chạy
  motor_isChange = false;
  servo_trai.write(40 +(motor_servo>0? int(motor_servo*30/35):int(motor_servo*35/35)))  ;
  servo_phai.write(70 + motor_servo);
  if(motor_isUse){
    led_isUse = true;   
    if(motor_isTien){
      if(motor_vanToc_trai - motor_vanToc_phai > 50)  led_type = 4;
      else if(motor_vanToc_phai - motor_vanToc_trai > 50) led_type = 3;
      else led_type = 1;
      motor_1_Tien(motor_vanToc_trai);
      motor_2_Tien(motor_vanToc_phai);
    }else{
      led_type = 2;
      motor_1_Lui(motor_vanToc_trai);
      motor_2_Lui(motor_vanToc_phai);  
      } 
    led_isChangeType = true;  
  }else{ // dừng xe
    digitalWrite(IN1,LOW);      digitalWrite(IN2,LOW);      digitalWrite(IN3,LOW);      digitalWrite(IN4,LOW); led_isUse = false; led_isChangeType = true;
    servo_trai.write(40)  ;
    servo_phai.write(70);
  }    
  }
}



bool motor_controlTraightProcessWork_isUse = false;
volatile float motor_controlTraight_distance_trai = 0;// khoảng cách đã đi được;
volatile float motor_controlTraight_distance_trai_2 = 0;// khoảng cách đã đi được;
void motor_controlTraightProcessWork(){
  if(!motor_controlTraightProcessWork_isUse) return;
  motor_controlTraight_distance_trai = distance_get(3);
  motor_controlTraight_distance_trai_2 = distance_get(6);
  
  //kiểm tra nếu == 0 thì return lấy giá trị vào motor_controlTraight_distance_trai
  if(motor_controlTraight_distance_trai == 0 && motor_controlTraight_distance_trai_2 == 0) return;
  if(motor_controlTraight_distance_trai ==0 ) motor_controlTraight_distance_trai = motor_controlTraight_distance_trai_2;
  else{motor_controlTraight_distance_trai = (motor_controlTraight_distance_trai + motor_controlTraight_distance_trai_2)/2;}
  
  // kiểm tra điều hướng 
  if(motor_controlTraight_distance_trai > 10 ){
    // bị lệch sang phải 
    if(motor_vanToc_phai -motor_vanToc_trai > 10 ) return;
    motor_servo  --;
    motor_vanToc_phai ++;
    motor_vanToc_trai --;
    motor_isChange = true;

  } else if(motor_controlTraight_distance_trai < 8 ){
    if(motor_vanToc_trai -motor_vanToc_phai > 10 ) return;
    motor_servo  ++;
    motor_vanToc_phai --;
    motor_vanToc_trai ++;
    motor_isChange = true;
  } else{
    motor_vanToc_phai = (motor_vanToc_phai + motor_vanToc_trai)/2;
    motor_vanToc_trai = motor_vanToc_phai;
    motor_servo = 0;
  }
  
  // servo_trai.write(40 +(motor_control_servo>0? int(motor_control_servo*30/35):int(motor_control_servo*35/35)));
  // servo_phai.write(70 + motor_control_servo);
  Serial.print("DisTance:"); Serial.print(motor_controlTraight_distance_trai);
  //Serial.print("servo:"); Serial.print(motor_control_servo);
  Serial.print("Trai:"); Serial.print(motor_vanToc_trai);
  Serial.print("Phai:"); Serial.println(motor_vanToc_phai); 
}
volatile long motor_checkCm = 0;
bool motor_checkStraight(int cm){
  if((((motor_encoder_phai + motor_encoder_trai)/2) - motor_checkCm )>(77*cm)) return true;
  if((((motor_encoder_phai + motor_encoder_trai)/2) - motor_checkCm )<(-77*cm)) return true;
  return false;
}
void motor_checkStraight_set(){
  motor_checkCm = (motor_encoder_phai + motor_encoder_trai)/2;
}
volatile long motor_checkAngle_trai = 0;
volatile long motor_checkAngle_phai = 0;
bool motor_checkAngle(int Angle){
  if((((motor_encoder_phai - motor_checkAngle_phai)+(motor_encoder_trai-motor_checkAngle_trai))/2)>(16*Angle))return true;
  if((((motor_encoder_phai - motor_checkAngle_phai)+(motor_encoder_trai-motor_checkAngle_trai))/2)<(-16*Angle))return true;
  return false;
}
void motor_checkAngle_set(){
  motor_checkAngle_trai = motor_encoder_trai;
  motor_checkAngle_phai =  motor_encoder_phai;
}

//hàm main vào cổng
bool main_passCong_isDoing = false;
bool main_passCong_isDone = false;
bool main_passCong_isFirst = true;
void main_passCong(){
  if(!main_passCong_isDoing) return;
  if(main_passCong_isFirst){
    motor_vanToc_trai =100;
    motor_vanToc_phai = 100;
    motor_servo = 0;
    motor_checkAngle_set();
    motor_checkStraight_set();
    motor_isUse =true;
    motor_isChange  = true;
    motor_controlTraightProcessWork_isUse = true;
    main_passCong_isFirst = false;
  }
  if ( motor_checkStraight(50) ){//}&& distance_get(4)>20){
    motor_isUse = false;
    motor_isChange = true;
    motor_controlTraightProcessWork_isUse = false;
    main_passCong_isDone = true;    
  }
  
}

void bluetooth_ProcessWork(){
  if(Serial.available()){
  char read = Serial.read();
  if(read == 'z'){ //'z' kí tự bắt đầu thông tin nhận từ andoid
    char check[2];
    Serial.readBytes(check,1);
    check[1] = '\0';
    Serial.println(check[0]);
    led_isUse =true;
    switch(check[0]){
      case '1':
      motor_isUse = false;
      motor_isChange = true;
      motor_controlTraightProcessWork_isUse = false;
      break;
      case'8':
      amThanh_isUse = true;
      amThanh_type = 2;
      amThanh_isChange = true;
      break;
       case'q':
      motor_vanToc_trai =100;
      motor_vanToc_phai = 100;
      motor_servo = 0;
      motor_isUse =true;
      motor_isChange  = true;
      motor_controlTraightProcessWork_isUse = true;
      break;
       case'a':
        main_passCong_isDoing = true;
        main_passCong_isFirst = true;
      break;

    }   
  }
  return;
  }
  
  }

void setup()
{

  //cài đặt motor và encdoer
  pinMode(Encoder_Trai,INPUT_PULLUP);
  pinMode(Encoder_Phai,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Encoder_Trai), doEncoderTrai,CHANGE);
  attachInterrupt(digitalPinToInterrupt(Encoder_Phai) ,doEncoderPhai,CHANGE);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  // set up cam biến 
  pinMode(Truoc_Echo,INPUT); pinMode(Truoc_Trig,OUTPUT);
  pinMode(Sau_Echo_1,INPUT); pinMode(Sau_Trig_1,OUTPUT);
  pinMode(Sau_Echo_2,INPUT); pinMode(Sau_Trig_2,OUTPUT);
  pinMode(Trai_Echo_1,INPUT); pinMode(Trai_Trig_1,OUTPUT);
  pinMode(Trai_Echo_2,INPUT); pinMode(Trai_Trig_2,OUTPUT);
  pinMode(Phai_Echo_1,INPUT); pinMode(Phai_Trig_1,OUTPUT);
  pinMode(Phai_Echo_2,INPUT); pinMode(Phai_Trig_2,OUTPUT);
  // set up âm thanh
  pinMode(AmThanh,OUTPUT);
  //setupLed
  pinMode(Led_Do_1,OUTPUT);
  pinMode(Led_Do_2,OUTPUT);
  pinMode(Led_Vang_Trai_1,OUTPUT);
  pinMode(Led_Vang_Trai_2,OUTPUT);  
  pinMode(Led_Vang_Phai_1,OUTPUT);
  pinMode(Led_Vang_Phai_2,OUTPUT);
  pinMode(Led_Trang_1,OUTPUT);
  pinMode(Led_Trang_2,OUTPUT);
  
  //set up servo
  servo_trai.attach(ServoTruoc_Trai);
  servo_phai.attach(ServoTruoc_Phai);
  servo_phai.write(70);
  servo_trai.write(40);
  ///// Phần chính trong thư viện 
  //Khởi tạo serial ở mức baudrate 9600
  Serial.begin(9600); 


  
  //Khởi gạo class timer (design pattern singleton) - bắt buộc phải có trong hàm setup (trước khi khởi tạo các job)
  Timer::getInstance()->initialize();
  ////////////////// Khởi tạo kIndustryCalendar trước ////////////////// 
  //Khởi tạo lịch, các công việc phải được sắp xếp theo chiều thời gian tăng dần để tránh lỗi
  kIndustryCalendar::getInstance()->initialize();
  //Chạy lệnh job ở thời điểm 0 trong chu kỳ
  //kIndustryCalendar::getInstance()->addJob(job_vaoCong, waitjob_start);
  //***Gán số lần hoạt động của chu trình
  kIndustryCalendar::getInstance()->setProcessCount(0);
  
 
  ////////////////// Khởi tạo WorkScheduler //////////////////
  bluetooth_WorkScheduler = new  WorkScheduler(0UL, bluetooth_ProcessWork);
  motor_Check_WorkScheduler = new WorkScheduler(0UL,main_passCong);
  motor_WorkScheduler =     new  WorkScheduler(60UL, motor_ProcessWork);
  amThanh_WorkScheduler =   new  WorkScheduler(100UL, amThanh_ProcessWork);
  led_WorkScheduler =       new  WorkScheduler(200UL, led_ProcessWork);
  motor_controlTraight_WorkScheduler = new WorkScheduler(0UL,motor_controlTraightProcessWork);
  
}
void loop()
{
  //đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
  Timer::getInstance()->update();

// update lại các tiến trình
  bluetooth_WorkScheduler->update();
  motor_Check_WorkScheduler->update();
  amThanh_WorkScheduler->update();
  led_WorkScheduler->update();
  motor_WorkScheduler->update();
  motor_controlTraight_WorkScheduler->update();
  //cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
  Timer::getInstance()->resetTick();
}




