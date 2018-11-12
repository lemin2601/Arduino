#include<Arduino.h>
#include "CongViec.h"

CongViec::CongViec(){
  m_pin = 5;
  pinMode(m_pin, INPUT_PULLUP);
}


byte CongViec::check() {
  byte nowState = digitalRead(m_pin);
  byte res = (m_lastState == HIGH && nowState == LOW);
  m_lastState = nowState;
  return res;
}
