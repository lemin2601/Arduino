#include <IRremote.h> // thư viện hỗ trợ IR remote
//Motor Left
const int MotorLeft = 8;// anolog
const int MotorLeftin1 = 7;// digital 
const int MotorLeftin2 = 6;// digital
//Motor Right
const int MotorRight = 3;// anolog
const int MotorRightin1 = 5;// digital
const int MotorRightin2 = 4;// digital

const int receiverPin = 12; // chân digital 8 dùng để đọc tín hiệu
IRrecv irrecv(receiverPin); // tạo đối tượng IRrecv mới
decode_results results;// lưu giữ kết quả giải mã tín hiệu
int speeds = 200;
int times = 200;
void setup()
{
  Serial.begin(9600);// serial 
  irrecv.enableIRIn(); // start the IR receiver

  // put your setup code here, to run once:
  pinMode(MotorLeft,OUTPUT);pinMode(MotorLeftin1,OUTPUT);pinMode(MotorLeftin2,OUTPUT);
  pinMode(MotorRight,OUTPUT);pinMode(MotorRightin1,OUTPUT);pinMode(MotorRightin2,OUTPUT);

}
// translate IR signals
void translateIR()
{
//  switch(results.value)
//  {
//    case 0x10:  stateRED = !stateRED; 
//                digitalWrite(RED, stateRED); 
//                break;
//    case 0x810: stateYELLOW = !stateYELLOW; 
//                digitalWrite(YELLOW, stateYELLOW); 
//                break;
//    case 0x410: stateGREEN = !stateGREEN; 
//                digitalWrite(GREEN, stateGREEN);  
//                break;
//    case 0xA90: stateRED = stateYELLOW = stateGREEN = false;
//                digitalWrite(RED, 0);
//                digitalWrite(YELLOW, 0);
//                digitalWrite(GREEN, 0);
//  }

  switch(results.value)
  {
    // Control
    case 0x10:  MotorRunning(speeds,11); // tiến 
                break;
    case 0x810: MotorRunning(speeds,00); // lùi  
                break;
    case 0x410: MotorRunning(speeds,01); // trái   
                break;
    case 0xA90: MotorRunning(speeds,10); // phải 
                break;

    // chỉnh tốc độ
    case 0x10:  
                if(speeds < 250){
                  speeds += 10; // tiến 
                }
                break;
    case 0x810: 
                if(speeds > 50){
                  speeds -= 10; // tiến 
                } // lùi  
                break;
    // chỉnh thời gian
    
    case 0x10:  // tăng thời gian
                if(times > 1000){
                  times += 100;  
                } 
                break;
    case 0x810: // giảm thời gian
                if(times > 1000){
                  times += 100;  
                }
                break;
                
  }
}
void loop()
{
  if (irrecv.decode(&results)) // nếu nhận được tín hiệu
  {
    translateIR();
    Serial.println(results.value, HEX);
    delay(200);
    irrecv.resume(); // nhận giá trị tiếp theo
  }
}


void MotorRunning(int Speed,int state){// 11 tiến, 00 lùi, 01 trái, 10 phải
   switch(state){
      case 11: // tiến
        analogWrite(MotorLeft,Speed);analogWrite(MotorRight,Speed);
        digitalWrite(MotorLeftin1,HIGH);digitalWrite(MotorLeftin2,LOW);
        digitalWrite(MotorRightin1,HIGH);digitalWrite(MotorRightin2,LOW);
        
      break;
      case 00: // tiến
        analogWrite(MotorLeft,Speed);analogWrite(MotorRight,Speed);
        digitalWrite(MotorLeftin1,HIGH);digitalWrite(MotorLeftin2,LOW);
        digitalWrite(MotorRightin1,HIGH);digitalWrite(MotorRightin2,LOW);
        
      break;
      case 01: // Xoay trái
        analogWrite(MotorLeft,Speed);analogWrite(MotorRight,Speed);
        digitalWrite(MotorLeftin1,LOW);digitalWrite(MotorLeftin2,HIGH);
        digitalWrite(MotorRightin1,HIGH);digitalWrite(MotorRightin2,LOW);
        
      break;
      case 10: // Xoay phải
        analogWrite(MotorLeft,Speed);analogWrite(MotorRight,Speed);
        digitalWrite(MotorLeftin1,HIGH);digitalWrite(MotorLeftin2,LOW);
        digitalWrite(MotorRightin1,LOW);digitalWrite(MotorRightin2,HIGH);
        
      break;
   }
   delay(times);
   digitalWrite(MotorLeftin1,LOW);digitalWrite(MotorLeftin2,LOW);
   digitalWrite(MotorRightin1,LOW);digitalWrite(MotorRightin2,LOW);    
}
