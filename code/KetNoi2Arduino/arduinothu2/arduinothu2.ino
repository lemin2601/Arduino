int led = 13;
unsigned long time = 0;
unsigned long delayTime = 1000; //Thời gian delay (không dùng hàm delay)
 
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW); // Tắt led
}
 
 
void loop() {
  if (Serial.available()) {
    time = millis(); //cập nhập lại biến time để cho đèn sáng được trong khoảng delayTime
    digitalWrite(led, HIGH); // đèn led sáng
    while (Serial.available()) { //xóa hết bộ đệm
      Serial.read();
      delay(3);
    }
    Serial.println("Da nhan duoc"); //trả về lại cho arduino 1
  }
    
  if (millis() - time > delayTime)
    digitalWrite(led, LOW);
}
