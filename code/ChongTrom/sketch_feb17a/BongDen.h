#ifndef __LEEMIN_CAMBIEN__
#define __LEEMIN_CAMBIEN__
#include <inttypes.h>
class BongDen
{
private:
	bool _trangThai;
	int _pin;
	
	

	~BongDen();
public:

	BongDen(int pin);
	void changeState(bool state);
	bool getState();
	
};

#endif