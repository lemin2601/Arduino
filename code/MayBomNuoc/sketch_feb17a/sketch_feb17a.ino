#include "Timer.h"
#include "kIndustryCalendar.h"
#include "WorkScheduler.h"

//Phấn viết thêm
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;
//Phần danh cho LCD
LiquidCrystal_I2C lcd(0x27,16,2);

// viết cho keyboard
const byte rows = 4; //số hàng
const byte columns = 3; //số cột
int holdDelay = 100; //Thời gian trễ để xem là nhấn 1 nút nhằm tránh nhiễu
int n = 3; // 
int state = 0; //nếu state =0 ko nhấn,state =1 nhấn thời gian nhỏ , state = 2 nhấn giữ lâu
char key = 0;
//Định nghĩa các giá trị trả về
char keys[rows][columns] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'},
};
byte rowPins[rows] = {5, 6, 7, 8}; //Cách nối chân với Arduino
byte columnPins[columns] = {9, 10, 11}; 
//cài đặt thư viện keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
// End Phần viết thêm
bool isMotorTurnOn = false;
///////////////////////các workscheduler
WorkScheduler *lcd_WorkScheduler;      //Tiến trình phát âm thanh
WorkScheduler *keyboard_WorkScheduler;        //Tiến trình phát led
WorkScheduler *time_WorkScheduler;         //Tiến trình motor chạy

// variable cho clcd
volatile bool isUserLCD = false; // nếu không dùng thì cho hiển thị thời gian hiện tại

//variable cho control
int numOfMenu = 0;
String menu[2]={
  "  Edit Time   >",
  "< Edit Alert   "
};
int numOfMenuShow = 0;
int numOfTimeShow = 0;
int second1 = 12, minute1 = 0, hour1 = 12,time1 = 1;
int second2= 0 , minute2 = 0, hour2= 13,time2 = 1;


//0: hiện giờ
//1: thời gian bật;
//2: cài đặt thời gian
//3: thời gian bơm 1
//4: thời gian bơm 2
//5: thời gian bơm 3
//6: thời gian bơm 4
//7: thời gian bơm 5
//8: thời gian bơm 6
//9: thời gian bơm 7
int bomActive[7] = {1,1,0,0,0,1,1};
int bomHour[7] = {6,8,10,14,16,17,18};
int bomMinute[7] = {0,0,0,0,0,0,0};
int hourTemp = 0,minuteTemp = 0,dayTemp =  0,monthTemp = 0,yearTemp = 0;
int timeActive = 10;

int menuShow = 0;
int cursorSetup = 0;
bool menuBlink = true;
bool volatile phimKichHoat = false;
long timeforDelayKey = 0;

bool volatile henGioMotor= false;
bool volatile controlMotor = false;

int getTime(){
  int i = 0;
  for(i = 0 ; i< 7; i ++){
    if(bomActive[i] == 0 ) continue;
    if(SoSanhTime(bomHour[i],bomMinute[i],hour,minute,bomHour[i],bomMinute[i] + timeActive)){
    return (timeActive+1+(bomHour[i]*60+bomMinute[i])-(hour*60+minute));
  }   
  }
  return 0;
}

/*Một số function viết thêm áp dụng bên dưới */
void printMenu(int numShow){
  // clear screen for the next loop:
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menu[numShow]);
  lcd.setCursor(0,1);
  lcd.print("*(Back)  #(Chon)");
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
 

/*End Một số function viết thêm áp dụng bên dưới */



 void lcdDigitalClockDisplay(){
   // digital clock display of the time
    lcd.setCursor(0,0);
    if(day<10) lcd.print("0");
  lcd.print(day);
    lcd.print("/");
    if(month<10) lcd.print("0");
  lcd.print(month);
    lcd.print(" ");
    if(hour<10) lcd.print("0");
  lcd.print(hour);
    lcd.print(":");
    if(minute<10) lcd.print("0");
  lcd.print(minute);
    lcd.print(":");
    if(second<10) lcd.print("0");
  lcd.print(second);
  lcd.print("  ");
  

}

void lcd_ProcessWork(){ 
  if( millis() - timeforDelayKey >  250){
  timeforDelayKey = millis();
  phimKichHoat = false;
  }
  switch(menuShow){
    case 0:
      lcd.setCursor(0,0);
      lcdDigitalClockDisplay();
      lcd.setCursor(0,1);
      lcd.print("Menu(1)Motor:");     
    
      if(isMotorTurnOn){
     
      if(checkTime()){
         if(getTime()<10) lcd.print("  ");
         else if(getTime()<100) lcd.print(" ");
        lcd.print(getTime());
      }else if(henGioMotor){
        if((1+(hour1*60+minute1) - (hour*60 + minute))< 10){lcd.print("  ");}
        else if(1+(hour1*60+minute1)- (hour*60 + minute)< 100){lcd.print(" ");}
        lcd.print((hour1*60+minute1)+1 - (hour*60 + minute));
      }
      else{
        lcd.print(" ON");
      }   
      }else{
        lcd.print("OFF");
      }
      

      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print(">>TG Bom     ");
      if(timeActive < 10) lcd.print(" ");
      lcd.print(timeActive+1);
      lcd.print("'");   
      lcd.setCursor(0,1);
      lcd.print("Chon(1)  Back(3)");
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print(">>SetTime       ");
      lcd.setCursor(0,1);
      lcd.print("Chon(1)  Back(3)");
      break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:   
      lcd.setCursor(0,0);
      lcd.print(">Bom(");
      lcd.print((menuShow - 2));
      lcd.print(")");
      
      if(bomHour[menuShow - 3] <10) lcd.print("0");
      lcd.print(bomHour[menuShow - 3]);
      lcd.print(":");
      if(bomMinute[menuShow - 3] < 10) lcd.print("0");
      lcd.print(bomMinute[menuShow - 3]); 
      if(bomActive[menuShow - 3] == 0) lcd.print(" OFF");
      else lcd.print(" ON ");
      lcd.setCursor(0,1);
      lcd.print("Chon(1)  Back(3)");
      break;
    // case 4:
      // lcd.setCursor(0,0);
      // lcd.print(">>Bom (2)    ");
      // if(bomActive[1] == 0) lcd.print("TAT");
      // else lcd.print("BAT");
      // lcd.setCursor(0,1);
      // lcd.print("Chon(1)  Back(3)");
      // break;
    // case 5:
      // lcd.setCursor(0,0);
      // lcd.print(">>Bom (3)    ");
      // if(bomActive[2] == 0) lcd.print("TAT");
      // else lcd.print("BAT");
      // lcd.setCursor(0,1);
      // lcd.print("Chon(1)  Back(3)");
      // break;
    // case 6:
      // lcd.setCursor(0,0);
      // lcd.print(">>Bom (4)    ");
      // if(bomActive[3] == 0) lcd.print("TAT");
      // else lcd.print("BAT");
      // lcd.setCursor(0,1);
      // lcd.print("Chon(1)  Back(3)");
      // break;
    // case 7:
      // lcd.setCursor(0,0);
      // lcd.print(">>Bom (5)    ");
      // if(bomActive[4] == 0) lcd.print("TAT");
      // else lcd.print("BAT");
      // lcd.setCursor(0,1);
      // lcd.print("Chon(1)  Back(3)");
      // break;
    // case 8:
      // lcd.setCursor(0,0);
      // lcd.print(">>Bom (6)    ");
      // if(bomActive[5] == 0) lcd.print("TAT");
      // else lcd.print("BAT");
      // lcd.setCursor(0,1);
      // lcd.print("Chon(1)  Back(3)");
      // break;
    // case 9:
      // lcd.setCursor(0,0);
      // lcd.print(">>Bom (7)    TAT");
      // if(bomActive[6] == 0) lcd.print("TAT");
      // else lcd.print("BAT");
      // lcd.setCursor(0,1);
      // lcd.print("Chon(1)  Back(3)");
      // break;
    case 11:
      lcd.setCursor(0,0);
      lcd.print("CaiDat TG Bom");
      if(timeActive < 10) lcd.print(" ");
      lcd.print(timeActive+1);
      lcd.print("'");   
      lcd.setCursor(0,1);
      lcd.print("         Back(3)");
      break;
    case 12:
      lcd.setCursor(0,0);     
      if(dayTemp<10) lcd.print("0");
      lcd.print(dayTemp);
      lcd.print("/");
      if(monthTemp<10) lcd.print("0");
      lcd.print(monthTemp);
      lcd.print("/");
      lcd.print(yearTemp);
      lcd.print("Set(1)");
      lcd.setCursor(0,1);
      if(hourTemp<10) lcd.print("0");
      lcd.print(hourTemp);
      lcd.print(":");
      if(minuteTemp<10) lcd.print("0");
      lcd.print(minuteTemp);
      lcd.print("    Back(3)");
      menuBlink = !menuBlink;
      if(menuBlink) return;
      switch(cursorSetup){
        case 0:
          lcd.setCursor(0,0);
          lcd.print("__");
          break;
        case 1:
          lcd.setCursor(3,0);
          lcd.print("__");
          break;
        case 2:
          lcd.setCursor(6,0);
          lcd.print("____");
          break;
        case 3:
          lcd.setCursor(0,1);
          lcd.print("__");
          break;
        case 4:
          lcd.setCursor(3,1);
          lcd.print("__");
          break;
        case 5:
          lcd.setCursor(5,1);
          lcd.print("______");
          break;
        case 6:
          lcd.setCursor(11,1);
          lcd.print("______");
          break;        
      }
      break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    
      lcd.setCursor(0,0);     
      if(bomHour[menuShow - 13] <10) lcd.print("0");
      lcd.print(bomHour[menuShow - 13]);
      lcd.print(":");
      if(bomMinute[menuShow - 13] < 10) lcd.print("0");
      lcd.print(bomMinute[menuShow - 13]);
      if(bomActive[menuShow - 13] == 0) lcd.print("     OFF   ");
      else lcd.print("     ON    ");
      lcd.setCursor(0,1);;
      lcd.print("         Back(3)");
      menuBlink = !menuBlink;
      if(menuBlink) return;
      switch(cursorSetup){
        case 0:
          lcd.setCursor(0,0);
          lcd.print("__");
          break;
        case 1:
          lcd.setCursor(3,0);
          lcd.print("__");
          break;
        case 2:
          lcd.setCursor(10,0);
          lcd.print("___");
          break;
      }
      break;
  }
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
bool checkTime(){
  // if(6<= month){
    // if(SoSanhTime(17,50,hour,minute,17,51)||SoSanhTime(17,53,hour,minute,17,54)) return true;
  // }
  // if(1<= month && month <=3){
    // if(SoSanhTime(8,0,hour,minute,8,10)||SoSanhTime(11,0,hour,minute,11,10)) return true;
  // }
  // if(month == 4){
    // if(SoSanhTime(8,0,hour,minute,8,10)
      // ||SoSanhTime(11,0,hour,minute,11,10)
      // ||SoSanhTime(16,0,hour,minute,16,10)) return true;
  // }
  // if(5<= month &&month <= 7){
    // if(SoSanhTime(5,30,hour,minute,5,40)
      // ||SoSanhTime(6,30,hour,minute,6,40)
      // ||SoSanhTime(11,0,hour,minute,11,10)
      // ||SoSanhTime(12,0,hour,minute,12,10)
      // ||SoSanhTime(16,0,hour,minute,16,10)) return true;
  // }
  // if(month == 8){
    // if(SoSanhTime(8,0,hour,minute,8,10)
      // ||SoSanhTime(11,0,hour,minute,11,10)
      // ||SoSanhTime(16,0,hour,minute,16,10)) return true;
  // }
  // if(month == 9){
    // if(SoSanhTime(8,0,hour,minute,8,10)
      // ||SoSanhTime(16,0,hour,minute,16,10)) return true;
  // }
  // if(10<= month && month <=12){
    // if(SoSanhTime(8,0,hour,minute,8,10)) return true;
  // }
  int i = 0;
  for(i = 0 ; i< 7; i ++){
    if(bomActive[i] == 0 ) continue;
    if(SoSanhTime(bomHour[i],bomMinute[i],hour,minute,bomHour[i],bomMinute[i] + timeActive)) return true;
  }
  return false;
}

void time_ProcessWork(){ //motor_vanToc_trai ; motor_vanToc_phai ;  motor_isUse ; motor_isChange ;motor_servo
  //Đọc dữ liệu module I2C thời gian thực và cập nhật biến 
  /* Đọc dữ liệu của DS1307 */
  readDS1307();
  
  // check thời gian 
 if(!controlMotor){
  if(henGioMotor){
    if((hour*60 + minute) > (hour1*60+ minute1)){
    henGioMotor = false;
    digitalWrite(2, LOW); 
    isMotorTurnOn = false;
    }
    return;
  }else{
    if(checkTime()){
    digitalWrite(2, HIGH); 
    isMotorTurnOn = true;   
    }else {
      digitalWrite(2, LOW); 
      isMotorTurnOn = false;  
    }
  }
  
   
}

}
void checkTempTime(){
  if(minuteTemp > 59){
    minuteTemp = 0;
  }
  if(minuteTemp < 0){minuteTemp = 59;}
  if(hourTemp > 23){
    hourTemp = 0;
  }
  if(hourTemp < 0){ hourTemp = 23;}
  
  if(dayTemp < 0){dayTemp = 31;}
  if(dayTemp >31 ){dayTemp = 0;}
  
  if(monthTemp > 12) monthTemp = 0;
  if(monthTemp < 0) monthTemp = 12;
}
void keyboard_ProcessWork(){
  char temp = keypad.getKey();

      
  if ((int)keypad.getState() ==  PRESSED) {
      
     if(phimKichHoat) return;
     phimKichHoat = true;
     Serial.println(key+" press");
    if (temp != 0) {
    key = temp;
    // bat tắt thủ công
    if(key == '*'){
      digitalWrite(2, LOW); 
    henGioMotor = false;
    controlMotor = false;
      isMotorTurnOn = false;
    }
    if(key == '#'){
      digitalWrite(2, HIGH); 
    controlMotor = true;    
      isMotorTurnOn = true;
    }
    if(key == '0'){
      hour1 = hour;
      minute1 = minute + timeActive;      
      digitalWrite(2, HIGH);  
      isMotorTurnOn = true;
      henGioMotor = true;
    }
  if(key == '7'){
      hour1 = hour;
      minute1 = minute + 7;      
      digitalWrite(2, HIGH);  
      isMotorTurnOn = true;
      henGioMotor = true;
    }
  
  if(key == '9'){
      hour1 = hour;
      minute1 = minute + 9;      
      digitalWrite(2, HIGH);  
      isMotorTurnOn = true;
      henGioMotor = true;
    }
    if( 0== menuShow){
      if(key == '1'){
        menuShow =1;
      }     
      return;
    }
    
    // thời gian bơm
    if(11 == menuShow){
      if(key == '2' || key == '6'){
        timeActive ++;
      }
      if(key == '4' || key == '8'){
        timeActive --;
      }
    }
    // set thời gian hệ thống
    if(12 == menuShow){
      if(key == '2'){
        switch (cursorSetup){
          case 0: 
            dayTemp ++;
            break;
          case 1:
            monthTemp ++;
            break;
          case 2:
            yearTemp ++;
            break;
          case 3:
            hourTemp ++;
            break;
          case 4:
            minuteTemp ++;
            break;
        }
      }
      if(key == '8'){
        switch (cursorSetup){
          case 0: 
            dayTemp --;
            break;
          case 1:
            monthTemp --;
            break;
          case 2:
            yearTemp --;
            break;
          case 3:
            hourTemp --;
            break;
          case 4:
            minuteTemp --;
            break;
        }
      }
      if(key == '4') {cursorSetup --; if(cursorSetup < 0) cursorSetup = 4;}
      if(key == '6') {cursorSetup ++;if(cursorSetup > 4) cursorSetup = 0;}
      if(key == '1') {
        setTime(hourTemp, minuteTemp, 0, 1, dayTemp, monthTemp, yearTemp - 2000); // 12:30:45 CN 08-02-2015
        menuShow -=10;
        return;
      }
    checkTempTime();
    }
    
    //set thời gian bơm 1;
    if( 13 <= menuShow && menuShow <= 19){
      if(key == '2'){
        switch(cursorSetup){
          case 0:
            bomHour[menuShow - 13] ++;
            if(bomHour[menuShow - 13] < 0) bomHour[menuShow - 13] = 23;
            if(bomHour[menuShow - 13] > 23) bomHour[menuShow - 13] = 0;
            break;
          case 1:
            bomMinute[menuShow - 13] ++;
            if(bomMinute[menuShow - 13] < 0) bomMinute[menuShow - 13] = 59;
            if(bomMinute[menuShow - 13] > 59) bomMinute[menuShow - 13] = 0;
            break;
          case 2:
            bomActive[menuShow - 13] ++;
            bomActive[menuShow - 13] %= 2;
            break;
        }
      }
      if(key == '8'){
        switch(cursorSetup){
          case 0:
            bomHour[menuShow - 13] --;
            if(bomHour[menuShow - 13] < 0) bomHour[menuShow - 13] = 23;
            if(bomHour[menuShow - 13] > 23) bomHour[menuShow - 13] = 0;
            break;
          case 1:
            bomMinute[menuShow - 13] --;
            if(bomMinute[menuShow - 13] < 0) bomMinute[menuShow - 13] = 59;
            if(bomMinute[menuShow - 13] > 59) bomMinute[menuShow - 13] = 0;
            break;
          case 2:
            bomActive[menuShow - 13] --;
            bomActive[menuShow - 13] %= 2;
            break;
        }
      }
      if(key == '4') {cursorSetup --; if(cursorSetup < 0) cursorSetup = 2;}
      if(key == '6') {cursorSetup ++;if(cursorSetup > 2) cursorSetup = 0;}
      
    }
    if(1 <= menuShow && menuShow <= 9){
      //trường hợp chọn menu
      if(key == '2' || key == '6'){
        menuShow ++;        
        if(menuShow > 9) menuShow = 1;
      }
      if(key == '4' || key == '8'){
        menuShow --;
        if(menuShow < 1) menuShow = 9;
      }
      if(key =='1'){        
        menuShow += 10;
        switch(menuShow){
          case 12: 
            hourTemp = hour;
            minuteTemp = minute;
            dayTemp = day;
            monthTemp = month;
            yearTemp = year;
            break;
        }
      }
    } 
  
    if( 0 < menuShow && 10 > menuShow){ // xử lý phim back
      if(key == '3'){ menuShow = 0;
      return;
      }
    }
    if(10< menuShow && 20 > menuShow){ // xử lý phim back
      if(key == '3'){
        menuShow -= 10;
        return;
      }
    }
    }
  
  }
  if ((int)keypad.getState() ==  HOLD) {
     //  if(phimKichHoat) return;
     phimKichHoat = true;
     Serial.println(key+" press");
    if (temp != 0) {
    key = temp;
    // bat tắt thủ công
    if(key == '*'){
      digitalWrite(2, LOW); 
      isMotorTurnOn = false;
    }
    if(key == '#'){
      digitalWrite(2, HIGH);  
      isMotorTurnOn = true;
    }
    if(key == '0'){
      hour1 = hour;
      minute1 = minute + timeActive;
      
      digitalWrite(2, HIGH);  
      isMotorTurnOn = true;
      henGioMotor = true;
    }
    if( 0== menuShow){
      if(key == '1'){
        menuShow =1;
      }     
      return;
    }
    
    // thời gian bơm
    if(11 == menuShow){
      if(key == '2' || key == '6'){
        timeActive ++;
      }
      if(key == '4' || key == '8'){
        timeActive --;
      }
    }
    // set thời gian hệ thống
    if(12 == menuShow){
      if(key == '2'){
        switch (cursorSetup){
          case 0: 
            dayTemp ++;
            if(dayTemp > 31) dayTemp = 0;
            break;
          case 1:
            monthTemp ++;
            if(monthTemp > 12) monthTemp = 0;
            break;
          case 2:
            yearTemp ++;
            break;
          case 3:
            if(hourTemp > 23) hourTemp = 0;
            hourTemp ++;
            break;
          case 4:
            if(minuteTemp > 59) minuteTemp = 0;
            minuteTemp ++;
            break;
        }
      }
      if(key == '8'){
        switch (cursorSetup){
          case 0: 
            
            dayTemp --;
            if(dayTemp < 0) dayTemp = 31;
            break;
          case 1:
            monthTemp --;
            if(monthTemp < 0) monthTemp = 12;
            break;
          case 2:
            yearTemp --;
            break;
          case 3:
            hourTemp --;
            if(hourTemp < 0) hourTemp = 23;
            break;
          case 4:
            minuteTemp --;
            
            if(minuteTemp < 0) minuteTemp = 59;
            break;
        }
      }
      if(key == '4') {cursorSetup --; if(cursorSetup < 0) cursorSetup = 4;}
      if(key == '6') {cursorSetup ++;if(cursorSetup > 4) cursorSetup = 0;}
      if(key == '1') {
        setTime(hourTemp, minuteTemp, 0, 1, dayTemp, monthTemp, yearTemp - 2000); // 12:30:45 CN 08-02-2015
        menuShow -=10;
        return;
      }
    }
    
    //set thời gian bơm 1;
    if( 13 <= menuShow && menuShow <= 19){
      if(key == '2'){
        switch(cursorSetup){
          case 0:
            bomHour[menuShow - 13] ++;
            if(bomHour[menuShow - 13] < 0) bomHour[menuShow - 13] = 23;
            if(bomHour[menuShow - 13] > 23) bomHour[menuShow - 13] = 0;
            break;
          case 1:
            bomMinute[menuShow - 13] ++;
            if(bomMinute[menuShow - 13] < 0) bomMinute[menuShow - 13] = 59;
            if(bomMinute[menuShow - 13] > 59) bomMinute[menuShow - 13] = 0;
            break;
          case 2:
            bomActive[menuShow - 13] ++;
            bomActive[menuShow - 13] %= 2;
            break;
        }
      }
      if(key == '8'){
        switch(cursorSetup){
          case 0:
            bomHour[menuShow - 13] --;
            if(bomHour[menuShow - 13] < 0) bomHour[menuShow - 13] = 23;
            if(bomHour[menuShow - 13] > 23) bomHour[menuShow - 13] = 0;
            break;
          case 1:
            bomMinute[menuShow - 13] --;
            if(bomMinute[menuShow - 13] < 0) bomMinute[menuShow - 13] = 59;
            if(bomMinute[menuShow - 13] > 59) bomMinute[menuShow - 13] = 0;
            break;
          case 2:
            bomActive[menuShow - 13] --;
            bomActive[menuShow - 13] %= 2;
            break;
        }
      }
      if(key == '4') {cursorSetup --; if(cursorSetup < 0) cursorSetup = 2;}
      if(key == '6') {cursorSetup ++;if(cursorSetup > 2) cursorSetup = 0;}
      
    }
    if(1 <= menuShow && menuShow <= 9){
      //trường hợp chọn menu
      if(key == '2' || key == '6'){
        menuShow ++;        
        if(menuShow > 9) menuShow = 1;
      }
      if(key == '4' || key == '8'){
        menuShow --;
        if(menuShow < 1) menuShow = 9;
      }
      if(key =='1'){        
        menuShow += 10;
        switch(menuShow){
          case 12: 
            hourTemp = hour;
            minuteTemp = minute;
            dayTemp = day;
            monthTemp = month;
            yearTemp = year;
            break;
        }
      }
    } 
  
    if( 0 < menuShow && 10 > menuShow){ // xử lý phim back
      if(key == '3'){ menuShow = 0;
      return;
      }
    }
    if(10< menuShow && 20 > menuShow){ // xử lý phim back
      if(key == '3'){
        menuShow -= 10;
        return;
      }
    }
    
    
    }
  
  }
  if ((int)keypad.getState() ==  RELEASED) {
    Serial.println(key+" release");
    phimKichHoat = false;
  }
}

void setup()
{
  // Phần viết thêm setup
  Wire.begin();
  /* cài đặt thời gian cho module */
  //setTime(12, 30, 45, 1, 8, 2, 15); // 12:30:45 CN 08-02-2015
  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình, cũng giống như dht.begin() trong chương trình trên  
  lcd.backlight();   //Bật đèn nền
  lcd.setCursor(0,0);
  lcd.print("Testing LCD ... ");

  pinMode(2, OUTPUT);
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
  
  ////////////////// Khởi tạo WorkScheduler //////////////////
  lcd_WorkScheduler = new  WorkScheduler(200UL, lcd_ProcessWork);
  keyboard_WorkScheduler = new WorkScheduler(10UL,keyboard_ProcessWork);
  time_WorkScheduler = new WorkScheduler(1000UL, time_ProcessWork);
  
}
void loop()
{
  //đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
  Timer::getInstance()->update();

 // update lại các tiến trình
  lcd_WorkScheduler->update();
  keyboard_WorkScheduler->update();
  time_WorkScheduler->update();
  //cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
  Timer::getInstance()->resetTick();
}



