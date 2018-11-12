#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(10, 11); // RX, TX
 
void setup() {
  Serial.begin(115200); //serial hardware kết nối với serial monitor của máy tính
 
  mySerial.begin(9600); //software serial kết nối với module sử dụng serial để giao tiếp (ở ví dụ này là bluetooth HC-06)
}
 
void loop() { 
  if (mySerial.available()) //khi software serial có dữ liệu
    Serial.write(mySerial.read()); //viết ra monitor
 
  if (Serial.available()) // khi hardware serial có dữ liệu
    mySerial.write(Serial.read()); //viết lại qua module bluetooth (software serial)
 
}
