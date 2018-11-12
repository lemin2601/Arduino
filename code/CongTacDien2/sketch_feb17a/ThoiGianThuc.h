#ifndef __ThoiGianThuc__
#define __ThoiGianThuc__

#include <Wire.h> 


class ThoiGianThuc {
private:
	/* Địa chỉ của DS1307 */
	const byte DS1307 = 0x68;
	/* Số byte dữ liệu sẽ đọc từ DS1307 */
	const byte NumberOfFields = 7;
	/* khai báo các biến thời gian */
	int second, minute, hour, day, wday, month, year;
public:
	ThoiGianThuc();
	void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr);
	byte getHour();
	byte getMinute();
	byte getSecond();
	byte getDay();
	byte getMonth();
	byte getYear();
	void setHour(int hour);
	void setMinute(int minute);
	void setDay(int day);
	void setMonth(int month);
	void setYear(int year);
	void update();	
	void print();
	void printLCD();
	
}; 
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

void printDigits(int digits){
    // các thành phần thời gian được ngăn chách bằng dấu :
    Serial.print(":");
        
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

ThoiGianThuc::ThoiGianThuc(){
	Wire.begin();
}

/* cài đặt thời gian cho DS1307 */
void ThoiGianThuc::setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
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
 
byte ThoiGianThuc::getHour(){
	return hour;
}
byte ThoiGianThuc::getMinute(){
	return minute;
}
byte ThoiGianThuc::getSecond(){
	return second;
}
byte ThoiGianThuc::getDay(){
	return day;
}
byte ThoiGianThuc::getMonth(){
	return month;
}
byte ThoiGianThuc::getYear(){
	return year;
}
void ThoiGianThuc::setHour(int h){
	hour = h;
	if(hour > 23) hour -=24;
	if(hour < 0) hour += 24;
	setTime(hour,minute, second, 1, day, month, year);
}
void ThoiGianThuc::setMinute(int m){
	minute = m;
	if(minute > 59) minute -=60;
	if(minute < 0) minute += 60;
	setTime(hour,minute, second, 1, day, month, year);
}
void ThoiGianThuc::setDay(int d){
	
	day = d;
	if(day > 31) day -=31;
	if(day < 0) day += 31;
	setTime(hour,minute, second, 1, day, month, year);
}
void ThoiGianThuc::setMonth(int m){
	month = m;
	if(month > 12) month -=12;
	if(month < 0) month += 12;
	setTime(hour,minute, second, 1, day, month, year);
}
void ThoiGianThuc::setYear(int y){
	year = y;
	if(year > 30) year = 15;
	setTime(hour,minute, second, 1, day, month, year);
}

void ThoiGianThuc::update(){
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
	//year += 2000;    
}
void ThoiGianThuc::print(){
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
void ThoiGianThuc::printLCD(){
}
 
#endif
