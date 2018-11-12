#include<Arduino.h>
#include "BongDen.h"

BongDen::BongDen(int pin) 
{
	_pin= pin;
	_trangThai = false;
	pinMode(pin,OUTPUT);	
	digitalWrite(_pin,_trangThai);
}

BongDen::~BongDen() { }

void BongDen::changeState(bool state)
{
	_trangThai = state;
	digitalWrite(_pin,_trangThai);
}
bool BongDen::getState()
{
	return _trangThai;
}
