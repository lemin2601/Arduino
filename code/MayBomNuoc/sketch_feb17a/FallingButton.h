#ifndef __FALLING_BUTTON__
#define __FALLING_BUTTON__

class FallingButton {
private:
	byte m_pin;
  byte m_lastState;
public:
	//khởi tạo falling button pin (digital read)
	FallingButton(const byte pin);
	
	
	byte check();
};

#endif
