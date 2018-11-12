#include "Timer.h"
#include "kIndustryCalendar.h"
#include "WorkScheduler.h"
#include "BongDen.h"
#include "CamBienTuBatDen.h"
#include "CamBienChongTrom.h"
//Phấn viết thêm
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
//DEFINE PIN CONTROL
const int trig = 8;     // chân trig của HC-SR04
const int echo = 7;     // chân echo của HC-SR04
volatile   int distance =0;  
int analogPin_CamBienChuyenDong = 0; 
int analogPin_CamBienChuyenDongChongTrom = 1; 

int val_CamBienChuyenDong = 0; 
const int Pin_Den = 10;
const int Pin_DenChinh = 6;
const int Pin_Dien = 5;

const int Pin_Chuong = 9;
const int Pin_Button = 11;

bool isUserLedLCD = true;
long timeDelayTurnOn = 0;
///////////////////////////////////////////

///////////////////////các workscheduler

WorkScheduler *lcd_WorkScheduler;       //Tiến trình hiển thị lcd
WorkScheduler *button_WorkScheduler;       //Tiến trình xu ly button
WorkScheduler *main_WorkScheduler;       //Tiến trình xu ly button

CamBienTuBatDen *camBienTuBatDen;
BongDen* bongDen;
BongDen* bongDenChinh;
BongDen* chuongCanhBao;
BongDen* dienGiat;

CamBienChongTrom *camBienChongTrom;


//Phần dành cho thời gian thực

/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;
 

long timeForDelayButton  = 0 ;
int countTimePressButton = 0;



void lcd_ProcessWork(){ 
  long val = 0;
  //lcd.clear();
  lcd.setCursor(0,0); 
  // trường hợp bất nút;
  //NumberOfPress: 0
  // Led: On| Use: ON
  lcd.print("   |   |   |    ");
  lcd.setCursor(0,1);
  lcd.print("   |   |   |    ");
  lcd.setCursor(0,0);
  if(camBienTuBatDen->getUse() == true) lcd.print("Yes"); else lcd.print("Not");
  
  lcd.setCursor(4,0);
  if(bongDen->getState() == true) lcd.print(" ON"); else lcd.print("OFF");
  
  lcd.setCursor(8,0);
  lcd.print(camBienTuBatDen->getVal());
  
  
  lcd.setCursor(0,1);
  if(camBienChongTrom->getUse() == true) lcd.print("Yes"); else lcd.print("Not");
  
  lcd.setCursor(4,1);
  lcd.print(camBienChongTrom->getChuyenDong());
  
  lcd.setCursor(8,1);
  lcd.print(camBienChongTrom->getKhoangCach());
  
  lcd.setCursor(12,1);
  if(camBienChongTrom->getState() == true)lcd.print(camBienChongTrom->getTimeKichHoat()); else lcd.print("NO");
  
  if(countTimePressButton > 0){
    lcd.setCursor(12,0);
    lcd.print(countTimePressButton);
    return;
  }
  
}

long timeForLedBlinkLCD = 0 ;
void button_ProcessWork(){ 
  bool stateUse;
  int buttonStatus = digitalRead(Pin_Button);    //Đọc trạng thái button
  if(millis() - timeForDelayButton < 500) return;
  if(buttonStatus == 0){
    if(camBienChongTrom->getState()){
      camBienChongTrom->setUse(false);
    }
    countTimePressButton ++;
    lcd.backlight();
    timeForDelayButton = millis();  
  }else{
    if(countTimePressButton>0) timeForLedBlinkLCD  = millis();  
    if(countTimePressButton == 1){
      stateUse = camBienTuBatDen->getUse();
      bongDen->changeState(stateUse);
      stateUse = !stateUse;
      camBienTuBatDen->setUse(stateUse);
      Serial.println(stateUse);
      // bật tắt đèn cơ bản
      
      
      if(camBienChongTrom->getState()){
        camBienChongTrom->setUse(false);  
      }   
      
    } 
    if(countTimePressButton == 2){
      if(camBienChongTrom->getUse()){
        camBienChongTrom->setUse(false);
      }else{
        camBienChongTrom->setUse(true);
      }
    }
    countTimePressButton = 0;
    if(millis()- timeForLedBlinkLCD > 2000) lcd.noBacklight();
  }
}

void main_ProcessWork(){ 
  camBienTuBatDen->update();
  camBienChongTrom->update();
}


void setup()
{
  //delay(1000);
  // Phần viết thêm setup
  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình, cũng giống như dht.begin() trong chương trình trên  
  lcd.backlight();   //Bật đèn nền

  pinMode(Pin_Chuong,OUTPUT);
  pinMode(Pin_Den,OUTPUT);
  pinMode(Pin_Button, INPUT_PULLUP);  //Cài đặt chân D2 ở trạng thái đọc dữ liệu
  dienGiat = new BongDen(Pin_Dien);
  bongDen = new BongDen(Pin_Den);
  bongDenChinh = new BongDen(Pin_DenChinh);
  chuongCanhBao = new BongDen(Pin_Chuong);
  camBienChongTrom = new CamBienChongTrom(trig,echo,analogPin_CamBienChuyenDong,chuongCanhBao,bongDenChinh,dienGiat);
  camBienTuBatDen = new CamBienTuBatDen(bongDen,analogPin_CamBienChuyenDong,15000);
  //setTime(10, 52, 30, 1, 23, 6, 17); // 12:30:45 CN 08-02-2015
 
  //End phần viết thêm setup
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
  
  ////////////////// Khởi tạo WorkScheduler //////////////////([thời gian delay giữa mỗi lần(milliseconds], process)
  lcd_WorkScheduler = new  WorkScheduler(100UL, lcd_ProcessWork);
  button_WorkScheduler = new  WorkScheduler(50UL, button_ProcessWork);
  main_WorkScheduler = new  WorkScheduler(0UL, main_ProcessWork);
  
}
void loop()
{
  //đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
  Timer::getInstance()->update();

 // update lại các tiến trình
  lcd_WorkScheduler->update();
  button_WorkScheduler->update();
  main_WorkScheduler->update();
  
  //cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
  Timer::getInstance()->resetTick();
}


void readDS1307()
{
        Wire.beginTransmission(DS1307);
        Wire.write((byte)0x00);
        Wire.endTransmission();
        Wire.requestFrom(DS1307, NumberOfFields);
        
        second = bcd2dec(Wire.read() & 0x7f);
        minute = bcd2dec(Wire.read() );
        hour   = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
        wday   = bcd2dec(Wire.read() );
        day    = bcd2dec(Wire.read() );
        month  = bcd2dec(Wire.read() );
        year   = bcd2dec(Wire.read() );
        year += 2000;    
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}
 
void digitalClockDisplay(){
    // digital clock display of the time
    Serial.print(hour);
    printDigits(minute);
    printDigits(second);
    Serial.print(" ");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(month);
    Serial.print(" ");
    Serial.print(year); 
    Serial.println(); 
}
 
void printDigits(int digits){
    // các thành phần thời gian được ngăn chách bằng dấu :
    Serial.print(":");
        
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
 
/* cài đặt thời gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00)); // đặt lại pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}


