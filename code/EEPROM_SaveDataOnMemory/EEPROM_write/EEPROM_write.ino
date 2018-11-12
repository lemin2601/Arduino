#include <EEPROM.h>
int addr = 0;//địa chỉ EEPROM mà ta sẽ lưu đầu tiên

void setup()
{
}

void loop()
{
  // cần phải chia giá trị của hàm analogRead cho 4
  // vì hàm analogRead trả về giá trị từ 0-1023 mà 
  // giá trị được lưu trong EEPROM chỉ từ 0-255 mà thôi
  int val = analogRead(0) / 4;
 
  // lưu giá trị vào ô nhớ addr của EEPROM.
  // Giá trị này sẽ được lưu giữ và vẫn còn đó
  // khi tắt Arduino
  EEPROM.write(addr, val);
 
  // Tăng giá trị ô nhớ lên 1
  addr = addr + 1;
 
  // chúng ta chỉ có 512 ô nhớ (số thứ tự từ 0-511)
  // nên khi addr (số thứ tự) == 512 thì ta quay về 
  // thứ tự là 0
  if (addr == 512)
    addr = 0;
 
  delay(5); // delay 5ms để trước khi lưu giá trị tiếp theo
}
