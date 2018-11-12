#include <EEPROM.h>

//bắt đầu đọc tại ô nhớ thứ 0
int address = 0;

byte value;

void setup()
{
  // Mở serial ở mức 9600 baudrate
  Serial.begin(9600);
  while (!Serial) {
    ; // Đợi Serial được kết nối thành công - chỉ cần thiết với 
      // mạch Arduino Leonardo
  }
}

void loop()
{
  // đọc giá trị tại ô nhớ hiện tại
  value = EEPROM.read(address);
 
  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
 
  // Tăng giá trị ô nhớ lên 1
  address = address + 1;
 
  // chúng ta chỉ có 512 ô nhớ (số thứ tự từ 0-511)
  // nên khi address (số thứ tự) == 512 thì ta quay về 
  // thứ tự là 0
  if (address == 512)
    address = 0;
   
  delay(5);// cần delay tối thiểu 5ms giữa mỗi lần đọc EEPROM!
}
