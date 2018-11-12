#include<Arduino.h>
#include "CamBienTuBatDen.h"

CamBienTuBatDen::CamBienTuBatDen(BongDen* bongden, int pinCamBien,int thoiGianSang) 
{
	isUse = true;
	_bongden = bongden;
	_pinCamBien = pinCamBien;
	_thoiGianSang = thoiGianSang;
	
}

CamBienTuBatDen::~CamBienTuBatDen() { }

void CamBienTuBatDen::update()
{
	if(!isUse) return;
	_valCamBien = analogRead(_pinCamBien);
	
	if(_valCamBien > 100){
		_thoiGianBat = millis();
		_bongden->changeState(true);
	}else{
		if(millis() - _thoiGianBat > _thoiGianSang){
			_bongden->changeState(false);
		}
	}
}
bool CamBienTuBatDen::getState()
{
	return _bongden->getState();
}
bool CamBienTuBatDen::getUse()
{
	return isUse;
}
void CamBienTuBatDen::setUse(bool use)
{
	isUse = use;
}
long CamBienTuBatDen::getVal()
{
	return _valCamBien;
}
int CamBienTuBatDen::getTimeRemain()
{
	if(_bongden->getState() == false)
	{
		return 0;
	}
	else{
		return (_thoiGianSang - (millis() - _thoiGianBat))/1000;
	}
}