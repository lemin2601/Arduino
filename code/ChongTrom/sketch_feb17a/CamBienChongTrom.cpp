#include<Arduino.h>
#include "CamBienChongTrom.h"

void CamBienChongTrom::getDistance()
{  
	detachInterrupt(0);

//    int distance;           // biến lưu khoảng cách
	
	/* Phát xung từ chân _pinTrig */
	digitalWrite(_pinTrig,0);   // tắt chân _pinTrig
	delayMicroseconds(2);
	digitalWrite(_pinTrig,1);   // phát xung từ chân _pinTrig
	delayMicroseconds(10);   // xung có độ dài 5 microSeconds
	digitalWrite(_pinTrig,0);   // tắt chân _pinTrig
	
	/* Tính toán thời gian */
	// Đo độ rộng xung HIGH ở chân _pinEcho. 
	_duration = pulseIn(_pinEcho,HIGH);  
	// Tính khoảng cách đến vật.
	_distance = int(_duration/2/29.412);
   
}


CamBienChongTrom::~CamBienChongTrom() 
{
}

CamBienChongTrom::CamBienChongTrom(int pinTrig,int pinEcho,int pinCamBien,BongDen* chuong,BongDen* den,BongDen* dienGiat)
{
	_trangThai = false;
	_pinCamBienChuyenDong = pinCamBien;
	_pinTrig = pinTrig;
	_pinEcho = pinEcho;
	_chuong = chuong;
	_bongDen = den;
	_dienGiat = dienGiat;
	pinMode(_pinTrig,OUTPUT);   // chân _pinTrig sẽ phát tín hiệu
	pinMode(_pinEcho,INPUT);    // chân _pinEcho sẽ nhận tín hiệu
	_valCamBienChuyenDong = 0;
	_thoiGianCho = 1000;
	_isCamBienChuyenDong = false;
	_isCamBienKhoangCach = false;
	_isUse = true;
	_bongDen->changeState(true);
}

void CamBienChongTrom::setUse(bool isUse)
{
	_isUse = isUse;
	if(isUse == false){
		_trangThai = false;
		_chuong->changeState(false);
		_bongDen->changeState(true);
		_dienGiatBlink = false;
		_dienGiat->changeState(false);
	}
}
void CamBienChongTrom::update()
{
	
	//đọc giá trị của siêu âm.
	_valCamBienChuyenDong = analogRead(_pinCamBienChuyenDong);
	getDistance();
	if(!_isUse) return;
	// nếu siêu âm lớn hơn giá trị đã định --> có trộm
	if(_valCamBienChuyenDong > 100)
	{
		_isCamBienChuyenDong = true;
		_thoiGianChuyenDong = millis();
	}else{
		if(millis()- _thoiGianChuyenDong > 2000){
			_isCamBienChuyenDong = false;
		}
	}
	if(_distance < _khoangCachTrom)
	{
		_isCamBienKhoangCach = true;
		_thoiGianKhoangCach = millis();
	}else{
		if(millis()- _thoiGianKhoangCach> 2000){
			_isCamBienKhoangCach = false;
		}
	}
	// thời gian kêu chuông > 5s --> bật điện, kích hoạt điện giật.
	if(_isCamBienChuyenDong && _isCamBienKhoangCach)
	{
		_trangThai = true;
		_thoiGianKichHoat = millis();
	}
	
	if(_trangThai)
	{
		_chuong->changeState(true);
		_dienGiatBlink = !_dienGiatBlink;
		_dienGiat->changeState(_dienGiatBlink);
		if((millis() - _thoiGianKichHoat)/1000> 5)
		{
			_bongDen->changeState(false);
		}
		// tối đa 5' bật nếu không thì reset.
		if((millis()  - _thoiGianKichHoat)/1000 > 180)
		{
			_trangThai = false;
			_chuong->changeState(false);
			_bongDen->changeState(true);
			_dienGiatBlink = false;
			_dienGiat->changeState(false);
		}
	}
	
}

bool CamBienChongTrom::getState()
{
		return _trangThai;
}	

bool CamBienChongTrom::getUse()
{
		return _isUse;
}	
int CamBienChongTrom::getTimeKichHoat()
{
	if(!_trangThai) return 0;
	return( (millis() - _thoiGianKichHoat)/1000);
}
int CamBienChongTrom::getKhoangCach()
{
	return _distance;
}
long CamBienChongTrom::getChuyenDong()
{
	return _valCamBienChuyenDong;
}
