#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //3RX, TX
void setup() {
  Serial.begin(9600);// Bạn khởi tạo một cổng Serial tại baudrate 9600. Ở Client, bạn cũng phải mở một cổng Serial cùng Baudrate với HOST
   mySerial.begin(9600);
}
 
int Step = 0;
 
void loop() {
  if (Step == 0) {
    Serial.println("LED_RED 1"); // Đèn đỏ sáng. 1 == HIGH
  } else if (Step == 1) {
    Serial.println("LED_RED 0"); // Đèn đỏ tắt . 0 == LOW
  } else if (Step == 2) {
    Serial.println("LED_BLUE 1");// Đèn xanh sáng
  } else if (Step == 3) {
    Serial.println("LED_BLUE 0");// Đèn xanh tắt
  }
  
  Step = (Step + 1) % 4; //Step sẽ tuần tự tuần hoàn các giá trị trong khoảng từ 0 --> 3
  
  delay(1000);// Dừng một giây giữa các lần gửi
}
