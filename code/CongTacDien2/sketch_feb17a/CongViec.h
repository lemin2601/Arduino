#ifndef __CONGVIEC__
#define __CONGVIEC__

class CongViec {
private:
	byte m_pin;
    byte m_lastState;
public:
	//khởi tạo falling button pin (digital read)
	CongViec();
	byte check();
};

#endif
