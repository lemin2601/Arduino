#include <EEPROM.h>

void setup()
{
  // xóa các giá trị tại các ô nhớ EEPROM từ 0-511 (có 512 ô nhớ)
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
    delay(5); //Phải có delay tối thiểu 5 mili giây giữa mối lần write
  }
   
  // Bật đèn led khi xóa xong 
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH); 
}

void loop()
{
}
