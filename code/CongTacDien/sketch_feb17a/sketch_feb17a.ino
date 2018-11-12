#include "Timer.h"
#include "kIndustryCalendar.h"
#include "WorkScheduler.h"

//Phấn viết thêm
#include <Wire.h> 
#include "PCD8544_HOANGSA.h"

//DEFINE PIN CONTROL
const int BUTTON_1 = 11;
const int BUTTON_2 = 10;
const int LED_LCD = 8;
const int PIN_CONTROL  =9;

///////////////////////////////////////////
//////THOI GIAN THUC
/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;
int hour1 = 0;
int minute1 = 0;
int second1 = 0;
//function for THOIGIANTHUC
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

////////////////////LCD
PCD8544 lcd(3,4,5,6,7);//RST,D/C, Din,CLK
//function for LCD
void printTIME(int x,int y,int hour,int minute,int second){
  if(hour <10){
  lcd.Asc_Char(x,y,'0',BLACK);
  lcd.Number_Long(x+6,y,hour,ASCII_NUMBER,BLACK);
  }else{
  lcd.Number_Long(x,y,hour,ASCII_NUMBER,BLACK);  
  }
  lcd.Asc_Char(x+15,y,':',BLACK);
  
  if(minute <10){
  lcd.Asc_Char(x+25,y,'0',BLACK);
  lcd.Number_Long(x+31,y,minute,ASCII_NUMBER,BLACK);
  }else{
  lcd.Number_Long(x+25,y,minute,ASCII_NUMBER,BLACK);  
  }
  lcd.Asc_Char(x+40,y,':',BLACK);
  
  if(second <10){
  lcd.Asc_Char(x+50,y,'0',BLACK);
  lcd.Number_Long(x+56,y,second,ASCII_NUMBER,BLACK);
  }else{
  lcd.Number_Long(x+50,y,second,ASCII_NUMBER,BLACK);  
  }
  

}

///////////////////////các workscheduler

WorkScheduler *lcd_WorkScheduler;       //Tiến trình hiển thị lcd
WorkScheduler *button_WorkScheduler;    //Tiến trình xử lý button
WorkScheduler *main_WorkScheduler;      //Tiến trình chính xử lý điều khiển

// VARIABLE FOR EVERYTHING HERE
bool ledLCD = false;
int menuSetup = 0;

void lcd_ProcessWork(){ 
  lcd.Clear();
  PrintMenu(menuSetup);
  lcd.Display();
  /* Hiển thị thời gian ra Serial monitor */
  //digitalClockDisplay();
}
void PrintMenu(int menuSetup){
  switch(menuSetup){  
    case 0:
      lcd.DrawLine(0,38,83,38,BLACK);         
      lcd.DrawLine(0,10,83,10,BLACK);   
      if(ledLCD)lcd.Asc_String(0,40,Asc("Time   offLED"), BLACK);       
      else lcd.Asc_String(0,40,Asc("Time    onLED"), BLACK);
      printTIME(10,15,hour,minute,second);
    break;
    case 1:     
      lcd.DrawLine(0,38,83,38,BLACK);     
        lcd.DrawLine(0,10,83,10,BLACK);   
      lcd.Asc_String(0,40,Asc("Set    Cancel"), BLACK);
      lcd.Asc_String(10,25,Asc("^^"), BLACK);     
      printTIME(10,0,hour,minute,second);
      printTIME(10,15,hour1,minute1,second1);     
     break;
     case 2:      
      lcd.DrawLine(0,38,83,38,BLACK);     
        lcd.DrawLine(0,10,83,10,BLACK);   
      lcd.Asc_String(0,40,Asc("Set    Cancel"), BLACK);
      lcd.Asc_String(35,25,Asc("^^"), BLACK);     
      printTIME(10,0,hour,minute,second);
      printTIME(10,15,hour1,minute1,second1);     
     break;
     case 3:      
      lcd.DrawLine(0,38,83,38,BLACK);     
        lcd.DrawLine(0,10,83,10,BLACK);   
      lcd.Asc_String(0,40,Asc("Set    Cancel"), BLACK);
      lcd.Asc_String(60,25,Asc("^^"), BLACK);     
      printTIME(10,0,hour,minute,second);
      printTIME(10,15,hour1,minute1,second1);     
     break;
     case 4:      
      lcd.DrawLine(0,38,83,38,BLACK);     
        lcd.DrawLine(0,10,83,10,BLACK);   
      lcd.Asc_String(0,40,Asc("Set<<  Cancel"), BLACK);
      printTIME(10,0,hour,minute,second);
      printTIME(10,15,hour1,minute1,second1);     
     break;
     case 5:      
      lcd.DrawLine(0,38,83,38,BLACK);     
        lcd.DrawLine(0,10,83,10,BLACK);   
      lcd.Asc_String(0,40,Asc("Set  >>Cancel"), BLACK);
      printTIME(10,0,hour,minute,second);
      printTIME(10,15,hour1,minute1,second1);     
     break;
  }
}
long timeForButton = 0;
void button_ProcessWork(){
    int btnMenu = digitalRead(BUTTON_1);
  int btnPlus = digitalRead(BUTTON_2);
  if(millis() - timeForButton < 350){
    return;
  }
  timeForButton = millis();
  if(menuSetup ==0 && btnPlus==0){    
    ledLCD = !ledLCD;
    digitalWrite(LED_LCD,ledLCD);
    Serial.print("change led");
  }
  if(btnMenu ==0){
    
    
    Serial.print(menuSetup);
    if(menuSetup == 0){
      hour1= hour;
      minute1 = minute;
      second1 = second;
    }
    if(menuSetup == 5) menuSetup = 1;
    else{menuSetup++; menuSetup %=6;}
    
  }
  
  if(menuSetup == 1 && btnPlus==0){   
    hour1 ++;
    hour1 %=24;
  }
  if(menuSetup == 2 && btnPlus==0){   
    minute1 ++;
    minute1 %=60;
  }
  if(menuSetup == 3 && btnPlus==0){   
    second1 ++;
    second1 %=60;
  }
  if(menuSetup == 4 && btnPlus==0){   
  
    setTime(hour1, minute1, second1, 1,  day, month, year); // 12:30:45 CN 08-02-2015
    menuSetup =0;
  }
  if(menuSetup == 5 && btnPlus==0){   
    menuSetup =0;
  }
  Serial.println(btnPlus);
  
}
bool SoSanhTime(int hour,int minute,int hour1,int minute1, int hour2,int minute2){
  if(hour < hour1 && hour1 < hour2) return true;
  if(hour > hour1 || hour1 > hour2) return false;
  if(hour == hour1 && hour1 == hour2){
    if(minute <= minute1 && minute1 <= minute2) return true;
    return false;
  }
  if(hour == hour1){
    if(minute <=minute1){
      if(hour1 <hour2)return true;
      if(hour1 == hour2 && minute1 <= minute2) return true;
      return false;
    }
    return false;
  }
  if(hour1 == hour2){
    if(minute1 <= minute2){
      if(hour < hour1)return true;
      if(hour == hour1 && minute <= minute1) return true;
      return false;
    }
    return false;   
  }
  return false;
}
void main_ProcessWork(){
  /* Đọc dữ liệu của DS1307 */
  readDS1307(); 
   //check thời gian
   if(SoSanhTime(17,30,hour,minute,23,59)||SoSanhTime(0,0,hour,minute,5,30)){
    digitalWrite(PIN_CONTROL,HIGH);
    return;
  }
  // if(SoSanhTime(22,0,hour,minute,23,59)||SoSanhTime(0,0,hour,minute,5,30)){
    // if(minute % 30 < 15) digitalWrite(PIN_CONTROL,HIGH);
    // else digitalWrite(PIN_CONTROL,LOW);
    // return;
  // }
  digitalWrite(PIN_CONTROL,LOW);
}


void setup()
{
  // Phần viết thêm setup
    Wire.begin();
  /* cài đặt thời gian cho module */
 // setTime(19, 48, 00, 1, 3, 11, 17); // 12:30:45 CN 08-02-2015
  lcd.ON();
  lcd.SET(50,0,0,0,4); 
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(LED_LCD, OUTPUT);
  pinMode(PIN_CONTROL, OUTPUT);
  //digitalWrite(LED_LCD,HIGH);
  
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
  lcd_WorkScheduler = new  WorkScheduler(200UL, lcd_ProcessWork);
  button_WorkScheduler = new WorkScheduler(0UL,button_ProcessWork);
  main_WorkScheduler = new WorkScheduler(1000UL, main_ProcessWork);
  
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




