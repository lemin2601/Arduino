
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
//0x27 là địa chỉ màn hình trong bus I2C. Phần này chúng ta không cần phải quá bận tâm vì hầu hết màn hình (20x4,...) đều như thế này!
//16 là số cột của màn hình (nếu dùng loại màn hình 20x4) thì thay bằng 20
//2 là số dòng của màn hình (nếu dùng loại màn hình 20x4) thì thay bằng 4
void setup() {
  Serial.begin(9600);
  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình, cũng giống như dht.begin() trong chương trình trên  
  lcd.backlight();   //Bật đèn nền
  lcd.setCursor(0,0);
  lcd.print("Toi Yeu Ardui NO!!");
}

void loop() {
}



