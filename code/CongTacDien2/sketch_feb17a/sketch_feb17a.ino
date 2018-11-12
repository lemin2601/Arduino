#include "Timer.h"
#include "kIndustryCalendar.h"
#include "WorkScheduler.h"
#include "CongViec.h"
#include "CongTacDien.h"
#include "ThoiGianThuc.h"
#include "FallingButton.h"
#include <Servo.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

///////////////////////các workscheduler
WorkScheduler *button_WorkScheduler;      //Tiến trình phát âm thanh
WorkScheduler *congTac_WorkScheduler;
WorkScheduler *LCD_WorkScheduler;

LiquidCrystal_I2C lcd(0x27,16,2);
CongTacDien *congTac;
ThoiGianThuc *thoiGian;
FallingButton *btnTr;
FallingButton *btnPh;
FallingButton *btnUp;
FallingButton *btnDown;
byte* time1;
byte numMenu = 0;;
byte ptrCursor = -1;
boolean isLedOn = true;
boolean isCursorView = false;
void button_ProcessWork(){
	if(btnTr->check()){
		ptrCursor ++;
		if(ptrCursor >=0 && ptrCursor < 7) numMenu = 1;
		if(ptrCursor >=7 && ptrCursor < 14) numMenu = 2;
		if(ptrCursor >=14 && ptrCursor < 21) numMenu = 3;
		if(ptrCursor >= 21 && ptrCursor <= 25) numMenu = 4;
		if(ptrCursor > 25){
			ptrCursor = -1;
			numMenu = 0;
		}
	}
	if(btnPh->check()){
		if(numMenu == 0){
			if(isLedOn){
				lcd.noBacklight();
				isLedOn = false;
			}else{
				lcd.backlight();
				isLedOn = true;
			}
		}else{
			numMenu = 0;
			ptrCursor = -1;
		}		
	}
	if(btnUp->check()){
		Serial.println(ptrCursor);
		if(0<=ptrCursor && ptrCursor<=20){
			switch(ptrCursor % 7){
				case 0:
					congTac->updateValue(ptrCursor,1);
					break;
				case 1:
					congTac->updateValue(ptrCursor,1);
					break;
				case 2:
					congTac->updateValue(ptrCursor,10);
					break;	
				case 3:
					congTac->updateValue(ptrCursor,1);
					break;
				case 4:
					congTac->updateValue(ptrCursor,10);
					break;
				case 5:
					congTac->updateValue(ptrCursor,10);
					break;			
				case 6:
					congTac->updateValue(ptrCursor,10);
					break;
			}	
		}
		else if(21<=ptrCursor && ptrCursor<=25){
			switch(ptrCursor){
				case 21:
					thoiGian->setDay(thoiGian->getDay() + 1);
					break;
				case 22:
					thoiGian->setMonth(thoiGian->getMonth() + 1);
					break;
				case 23:
					thoiGian->setYear(thoiGian->getYear() + 1);
					break;
				case 24:
					thoiGian->setHour(thoiGian->getHour() + 1);
					break;
				case 25:
					thoiGian->setMinute(thoiGian->getMinute() + 1);
					break;		
			}
		}
	}
	if(btnDown->check()){
		Serial.println(ptrCursor);
		if(0<=ptrCursor && ptrCursor<=20){
			switch(ptrCursor % 7){
				case 0:
					congTac->updateValue(ptrCursor,-1);
					break;
				case 1:
					congTac->updateValue(ptrCursor,-1);
					break;
				case 2:
					congTac->updateValue(ptrCursor,-10);
					break;	
				case 3:
					congTac->updateValue(ptrCursor,-1);
					break;
				case 4:
					congTac->updateValue(ptrCursor,-10);
					break;
				case 5:
					congTac->updateValue(ptrCursor,-10);
					break;			
				case 6:
					congTac->updateValue(ptrCursor,-10);
					break;
			}	
		}
		else if(21<=ptrCursor && ptrCursor<=25){
			switch(ptrCursor){
				case 21:
					thoiGian->setDay(thoiGian->getDay() -1);
					break;
				case 22:
					thoiGian->setMonth(thoiGian->getMonth() - 1);
					break;
				case 23:
					thoiGian->setYear(thoiGian->getYear() - 1);
					break;
				case 24:
					thoiGian->setHour(thoiGian->getHour() - 1);
					break;
				case 25:
					thoiGian->setMinute(thoiGian->getMinute() - 1);
					break;		
			}
		}
			
	}
}
void congTac_ProcessWork(){
	thoiGian->update();
	congTac->checkUpdateState(thoiGian->getHour(),thoiGian->getMinute());
}
void lcd_ProcessWork(){
	time1 = congTac->getTime();
	switch(numMenu){
		default:
		case 0:
			lcd.setCursor(0,0);
			lcd.print(" ");
			lcd.print(thoiGian->getDay()<10?"0":"");
			lcd.print(thoiGian->getDay());
			lcd.print("/");
			lcd.print(thoiGian->getMonth()<10?"0":"");
			lcd.print(thoiGian->getMonth());
			lcd.print(" ");
			lcd.print(thoiGian->getHour()<10?"0":"");
			lcd.print(thoiGian->getHour());
			lcd.print(":");
			lcd.print(thoiGian->getMinute()<10?"0":"");
			lcd.print(thoiGian->getMinute());
			lcd.print(":");
			lcd.print(thoiGian->getSecond()<10?"0":"");
			lcd.print(thoiGian->getSecond());
			lcd.print(" ");
			lcd.setCursor(0,1);
			lcd.print("CaiDat    ");
			if(isLedOn)lcd.print("TatLed"); else lcd.print("BatLed");
			break;
		case 1:
			lcd.setCursor(0,0);
			if(time1[0] == 1) lcd.print("1On "); else lcd.print("1off");
			if(time1[1] < 10) lcd.print("0"); lcd.print(time1[1]);
			lcd.print(":");
			if(time1[2] < 10) lcd.print("0"); lcd.print(time1[2]);
			lcd.print("->");
			if(time1[3] < 10) lcd.print("0"); lcd.print(time1[3]);
			lcd.print(":");
			if(time1[4] < 10) lcd.print("0"); lcd.print(time1[4]);
			lcd.setCursor(0,1);
			lcd.print("Next");
			if(time1[5] < 100) lcd.print("0"); if(time1[5] < 10) lcd.print("0"); lcd.print(time1[5]);
			lcd.print("--");
			if(time1[6] < 100) lcd.print("0"); if(time1[6] < 10) lcd.print("0"); lcd.print(time1[6]);
			lcd.print("Back");
			break;
		case 2:
			lcd.setCursor(0,0);
			if(time1[7] == 1) lcd.print("2On "); else lcd.print("2off");
			if(time1[8] < 10) lcd.print("0"); lcd.print(time1[8]);
			lcd.print(":");
			if(time1[9] < 10) lcd.print("0"); lcd.print(time1[9]);
			lcd.print("->");
			if(time1[10] < 10) lcd.print("0"); lcd.print(time1[10]);
			lcd.print(":");
			if(time1[11] < 10) lcd.print("0"); lcd.print(time1[11]);
			lcd.setCursor(0,1);
			lcd.print("Next");
			if(time1[12] < 100) lcd.print("0"); if(time1[12] < 10) lcd.print("0"); lcd.print(time1[12]);
			lcd.print("--");
			if(time1[13] < 100) lcd.print("0"); if(time1[13] < 10) lcd.print("0"); lcd.print(time1[13]);
			lcd.print("Back");
			break;
		case 3:
			lcd.setCursor(0,0);
			if(time1[14] == 1) lcd.print("3On "); else lcd.print("3off");
			if(time1[15] < 10) lcd.print("0"); lcd.print(time1[15]);
			lcd.print(":");
			if(time1[16] < 10) lcd.print("0"); lcd.print(time1[16]);
			lcd.print("->");
			if(time1[17] < 10) lcd.print("0"); lcd.print(time1[17]);
			lcd.print(":");
			if(time1[18] < 10) lcd.print("0"); lcd.print(time1[18]);
			lcd.setCursor(0,1);
			lcd.print("Next");
			if(time1[19] < 100) lcd.print("0"); if(time1[19] < 10) lcd.print("0"); lcd.print(time1[19]);
			lcd.print("--");
			if(time1[20] < 100) lcd.print("0"); if(time1[20] < 10) lcd.print("0"); lcd.print(time1[20]);
			lcd.print("Back");
			break;
		case 4:
			lcd.setCursor(0,0);
			lcd.print(thoiGian->getDay()<10?"0":"");
			lcd.print(thoiGian->getDay());
			lcd.print("/");
			lcd.print(thoiGian->getMonth()<10?"0":"");
			lcd.print(thoiGian->getMonth());
			lcd.print("/");
			lcd.print(thoiGian->getYear()<10?"0":"");
			lcd.print(thoiGian->getYear());
			lcd.print(" ");
			lcd.print(thoiGian->getHour()<10?"0":"");
			lcd.print(thoiGian->getHour());
			lcd.print(":");
			lcd.print(thoiGian->getMinute()<10?"0":"");
			lcd.print(thoiGian->getMinute());
			lcd.print("  ");
			lcd.setCursor(0,1);
			lcd.print("Next        Back");
			break;
	}
	if(numMenu > 0 && numMenu < 4) {
		if(isCursorView){
			switch(ptrCursor % 7){
				case 0:
					lcd.setCursor(1,0);
					lcd.print("___");
					break;
				case 1:
					lcd.setCursor(4,0);
					lcd.print("__");
					break;
				case 2:
					lcd.setCursor(7,0);
					lcd.print("__");
					break;	
				case 3:
					lcd.setCursor(11,0);
					lcd.print("__");
					break;
				case 4:
					lcd.setCursor(14,0);
					lcd.print("__");
					break;
				case 5:
					lcd.setCursor(4,1);
					lcd.print("___");
					break;			
				case 6:
					lcd.setCursor(9,1);
					lcd.print("___");
					break;
			}	
		}
		isCursorView = !isCursorView;
	}
	if(numMenu == 4) {
		if(isCursorView){
			switch(ptrCursor){
				case 21:
					lcd.setCursor(0,0);
					lcd.print("__");
					break;
				case 22:
					lcd.setCursor(3,0);
					lcd.print("__");
					break;
				case 23:
					lcd.setCursor(6,0);
					lcd.print("__");
					break;	
				case 24:
					lcd.setCursor(9,0);
					lcd.print("__");
					break;
				case 25:
					lcd.setCursor(12,0);
					lcd.print("__  ");
					break;
			}	
		}
		isCursorView = !isCursorView;
	}
}

void setup()
{
  //Khởi tạo serial ở mức baudrate 9600
  Serial.begin(9600); 
   pinMode(2,OUTPUT);
  congTac = new CongTacDien(2);
  thoiGian = new ThoiGianThuc();
  // btnTr = new FallingButton(4);
  // btnPh = new FallingButton(5);
  // btnUp = new FallingButton(6);
  // btnDown = new FallingButton(7);
    btnTr = new FallingButton(7);
  btnPh = new FallingButton(6);
  btnUp = new FallingButton(4);
  btnDown = new FallingButton(5);
  
  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình, cũng giống như dht.begin() trong chương trình trên  
  lcd.backlight();   //Bật đèn nền
	lcd.setCursor(0,0);
	lcd.print("Loading ...     "); 
	// thoiGian->setTime(16,47, 55, 1, 16, 12, 17);
	// congTac->setOnOff(0,1); // 0 off 1 on
	// congTac->setHour(0,1,01,1,05);
	// congTac->setTime(0,01,01);
	// congTac->setOnOff(1,0); // 0 off 1 on
	// congTac->setHour(1,11,12, 13,14);
	// congTac->setTime(1,15,16);
	// congTac->setOnOff(2,0); // 0 off 1 on
	// congTac->setHour(2,10,25, 13,45);
	// congTac->setTime(2,30,31);

	//thoiGian->setTime(22, 21, 30, 1, 13, 12, 17); 
  congTac->init();
  congTac->print();
  byte* time1 = congTac->getTime();
  for(int i =0;i< 10 ; i++){
    Serial.println(time1[i]);
  }
  Serial.print(time1[0]+"abc");
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
  button_WorkScheduler =   new  WorkScheduler(100UL, button_ProcessWork); //100UL là thời gian delay giữa mỗi lần lặp
  congTac_WorkScheduler = new WorkScheduler(100UL,congTac_ProcessWork);
  LCD_WorkScheduler = new WorkScheduler(250UL,lcd_ProcessWork);
}
void loop()
{
	//đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
	Timer::getInstance()->update();

	// update lại các tiến trình
	button_WorkScheduler->update();
	congTac_WorkScheduler->update();
	LCD_WorkScheduler->update();
	//cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
	Timer::getInstance()->resetTick();
}




