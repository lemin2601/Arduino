#ifndef __LEEMIN_CAMBIENCHONGTROM__
#define __LEEMIN_CAMBIENCHONGTROM__
#include <inttypes.h>
#include "BongDen.h"
class CamBienChongTrom
{
private:
	int _khoangCachTrom = 90;
	
	bool _isCamBienChuyenDong;
	bool _isCamBienKhoangCach;
	
	unsigned long _thoiGianChuyenDong;
	unsigned long _thoiGianKhoangCach;
	unsigned long _thoiGianKichHoat;
	unsigned long _duration; // biến đo thời gian
	int _distance;
	
	bool _trangThai;
	int _pinCamBienChuyenDong;
	int _pinTrig;
	int _pinEcho;
	long _valCamBienChuyenDong;
	unsigned long _thoiGianCho;
	
	BongDen *_chuong;
	BongDen *_bongDen;
	BongDen *_dienGiat;
	bool _dienGiatBlink;
	
	bool _isUse;
	
	~CamBienChongTrom();
	void getDistance();
public:

	CamBienChongTrom(int pinTrig,int pinEcho,int pinCamBien,BongDen* chuong,BongDen* den,BongDen* dienGiat);
	void update();	
	int getKhoangCach();
	long getChuyenDong();
	int getTimeKichHoat();
	void setUse(bool isUse);
	bool getUse();
	bool getState();
	
};

#endif