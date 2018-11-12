#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(14, 15); // RX, TX
 
void setup() {
  Serial.begin(9600);
 
  Serial.println("Da san sang");

 
  mySerial.begin(9600);
}
 
void loop() {
  // Khi máy tính gửi dữ liệu cho mình
  if (Serial.available() > 0) {
    Serial.print("May tinh gui: ");
    while (Serial.available() > 0) {// in hết nội dung mà máy tính gửi cho mình, đồng thời gửi cho arduino thứ 2  
      char ch = Serial.read(); //đọc ký tự đầu tiên trogn buffer
      Serial.write(ch); //xuất ra monitor máy tính
      mySerial.write(ch); //gửi dữ liệu cho Arduino thứ 2
      //delay(3);
    }    
    Serial.println();
  }
  
  if (mySerial.available() > 0) {
    Serial.println("Serial thu 2 gui gia tri: ");
    //đọc giá trị từ Arduino nếu có
    delay(20);
    while (mySerial.available() > 0) {
      char ch = mySerial.read(); //đọc
      Serial.write(ch); //xuất ra monitor
      delay(3);
    }
    Serial.println();
  }
}
