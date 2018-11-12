#include <SoftwareSerial.h>
SoftwareSerial mySerial1(15,14); //RX, TX
SoftwareSerial mySerial(17,16); //RX, TX
void setup() {
  Serial.begin(9600);// Bạn khởi tạo một cổng Serial tại baudrate 9600. Ở Client, bạn cũng phải mở một cổng Serial cùng Baudrate với HOST
   mySerial.begin(9600);
   mySerial1.begin(57600);
}
 
int Step = 0;
 void Client1(){
  if (Step == 0) {
    mySerial.println("LED_RED 1"); // Đèn đỏ sáng. 1 == HIGH

  } else if (Step == 1) {
    mySerial.println("LED_RED 0"); // Đèn đỏ tắt . 0 == LOW

  } else if (Step == 2) {
    mySerial.println("LED_BLUE 1");// Đèn xanh sáng

  } else if (Step == 3) {
    mySerial.println("LED_BLUE 0");// Đèn xanh tắt
  }
    Serial.println("client 1: send ->");
 }
 void Client2(){
  if (Step == 0) {
    mySerial1.println("LED_RED 1"); // Đèn đỏ sáng. 1 == HIGH

  } else if (Step == 1) {
    mySerial1.println("LED_RED 0"); // Đèn đỏ tắt . 0 == LOW

  } else if (Step == 2) {
    mySerial1.println("LED_BLUE 1");// Đèn xanh sáng

  } else if (Step == 3) {
    mySerial1.println("LED_BLUE 0");// Đèn xanh tắt
  }
    Serial.println("client 2   : send ->");
 }
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
 Client1();
 Client2();
  Step = (Step + 1) % 4; //Step sẽ tuần tự tuần hoàn các giá trị trong khoảng từ 0 --> 3
  
  delay(1000);// Dừng một giây giữa các lần gửi
}
