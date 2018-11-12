#ifndef __CongTacDien__
#define __CongTacDien__

#include <EEPROM.h>

class CongTacDien {
private:
	byte m_pin;
    byte m_state1[21];  //onOff, hourOn,minuteOn,hourOff,minuteOff,timeOn,timeOff
    bool m_isRunning = true;
    bool m_offOneTime = false;
    int address = 0;
public:
	//khởi tạo falling button pin (digital read)
	CongTacDien(const byte pin);
	void init();
	void checkUpdateState(byte hour,byte minute);
	void setOnOff(byte num,byte onOff); // 0 off 1 on
	void setHour(byte num,byte hourOn,byte minuteOn, byte hourOff,byte minuteOff);
	void setTime(byte num,byte timeOn,byte timeOff);
	void updateValue(byte index,byte value);
	void reset();
	void turnOFF();
	void turnON();
	void turnOFFOneTime();
  byte* getTime();
	void print();
};
bool checkIsInThatTime(byte hour,byte minute,byte hourOn,byte minuteOn, byte hourOff,byte minuteOff){
	if(hourOn < hourOff || ( hourOn == hourOff && minuteOn <= minuteOff)){
		// chua qua 24h
		if((hourOn < hour || ( hourOn == hour && minuteOn <= minute))&&(hour < hourOff || ( hour == hourOff && minute <= minuteOff))){
			return true;
		}
		else return false;
	}else{
		//qua 24h
		if((hourOn < hour || ( hourOn == hour && minuteOn <= minute))||(hour < hourOff || ( hour == hourOff && minute <= minuteOff))){
			return true;
		}
		else return false;
	}
}

bool checkIsTurnOn(byte hour,byte minute,byte hourOn,byte minuteOn, byte timeOn,byte timeOff){
	int total = 0;
	if(hourOn < hour || ( hourOn == hour && minuteOn <= minute)){
		// chua qua 24h
		total = (hour - hourOn) * 60 + (minute - minuteOn);
		total = total % (timeOn + timeOff);
		if(0 <= total && total < timeOn) return true;
		else return false;
	}else{
		//qua 24h
		total = (23 - hourOn) * 60 + (59 - minuteOn) + hour * 60 + minute;
		total = total % (timeOn + timeOff);
		if(0 <= total && total < timeOn) return true;
		else return false;
	}
}

void CongTacDien::updateValue(byte index, byte value){
	switch(index % 7){
		case 0:
			m_state1[index] += value;
			m_state1[index] %=2;
			break;
		case 1:
			m_state1[index] += value;
			if(m_state1[index] >= 24) m_state1[index] = 0;
			if(m_state1[index] < 0) m_state1[index] = 23;
			break;
		case 2:
			m_state1[index] += value;
			if(m_state1[index] >= 60) m_state1[index] = 0;
			if(m_state1[index] < 0) m_state1[index] = 59;
			break;	
		case 3:
			m_state1[index] += value;
			if(m_state1[index] >= 24) m_state1[index] = 0;
			if(m_state1[index] < 0) m_state1[index] = 23;
			break;
		case 4:
			m_state1[index] += value;
			if(m_state1[index] >= 60) m_state1[index] = 0;
			if(m_state1[index] < 0) m_state1[index] = 59;
			break;
		case 5:
			m_state1[index] += value;
			if(m_state1[index] >= 250) m_state1[index] = 0;
			if(m_state1[index] < 0) m_state1[index] = 250;
			break;			
		case 6:
			m_state1[index] += value;
			if(m_state1[index] >= 250) m_state1[index] = 0;
			if(m_state1[index] < 0) m_state1[index] = 250;
			break;
	}	
	EEPROM.write(index, m_state1[index]);
	
}
CongTacDien::CongTacDien(const byte pin){
	this->m_pin = pin;
}	

byte* CongTacDien::getTime(){
  return m_state1;
}
void CongTacDien::init(){
	//doc gia tri tu bo nho
  byte value =0;
	while(address < 512){
		value = EEPROM.read(address);
		this->m_state1[address] = value;
		delay(10);// c?n delay t?i thi?u 5ms gi?a m?i l?n ??c EEPROM!
		address ++;	
	}
}

void CongTacDien::checkUpdateState(byte hour,byte minute){
	//kiem tra gia tri va bat tat den
	if(!m_isRunning) return;
	boolean isOn = false;
	for(int i = 0 ;i< 3; i++){
		if(m_state1[i*7] == 1){
			
			if(checkIsInThatTime(hour,minute,m_state1[i*7+1],m_state1[i*7+2],m_state1[i*7+3],m_state1[i*7+4])){
				
				if(checkIsTurnOn(hour,minute,m_state1[i*7+1],m_state1[i*7+2],m_state1[i*7+5],m_state1[i*7+6])){
					//bat den
					isOn = true;
				}else{
					//tat den
					isOn = false;
				}
				break;
			}
		}
	}
	if(isOn)digitalWrite(m_pin, HIGH); else digitalWrite(m_pin, LOW);
	m_offOneTime = false;
}

void CongTacDien::reset(){
	//tra ve gia tri ban dau
	for (int i = 0; i < 512; i++) {
   		 EEPROM.write(i, 0);
    	delay(10); //Ph?i c� delay t?i thi?u 5 mili gi�y gi?a m?i l?n write
 	}
}

void CongTacDien::setOnOff(byte num,byte onOff){
	int index = 7*num;
	m_state1[index] = onOff;
	EEPROM.write(index, onOff);
}

void CongTacDien::setHour(byte num, byte hourOn, byte minuteOn,byte hourOff, byte minuteOff){
	int index = 7*num;
	m_state1[++index] = hourOn;
	EEPROM.write(index, hourOn);
	delay(10); 
	m_state1[++index] = minuteOn;
	EEPROM.write(index, minuteOn);
	delay(10); 
	m_state1[++index] = hourOff;
	EEPROM.write(index, hourOff);
	delay(10); 
	m_state1[++index] = minuteOff;
	EEPROM.write(index, minuteOff);
	delay(10); 
}

void CongTacDien::setTime(byte num,byte timeOn,byte timeOff){
	int index = 7*num + 4;
    m_state1[++index] = timeOn;
	EEPROM.write(index, timeOn);
	delay(10); 
	m_state1[++index] = timeOff;
	EEPROM.write(index, timeOff);
	delay(10); 
}

void CongTacDien::turnOFF(){
	m_isRunning = false;
}

void CongTacDien::turnON(){
	m_isRunning = true;
}

void CongTacDien::turnOFFOneTime(){
	m_offOneTime = true;
}

void CongTacDien::print(){
	for(int i =0;i< 21;i++){
		Serial.print(m_state1[i]);
	}
}
#endif
