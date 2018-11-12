#ifndef __LEEMIN_TUBATDEN__
#define __LEEMIN_TUBATDEN__
#include <inttypes.h>
#include "BongDen.h"

class CamBienTuBatDen
{
private:
	BongDen* _bongden;
	int _pinCamBien;
	long _valCamBien;
	unsigned long _thoiGianSang;
	unsigned long _thoiGianBat;
	bool isUse;
	
public:
	CamBienTuBatDen(BongDen* bongDen,int pinCamBien,int thoiGianSang);
	~CamBienTuBatDen();
	void update();
	bool getState();
	long getVal();
	void setUse(bool isUse);
	bool getUse();
	int getTimeRemain();
};

#endif