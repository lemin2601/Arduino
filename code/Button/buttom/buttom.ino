int button = 5;
int BUTTON_1= 10;
int BUTTOn_2 = 11;
int LED_LCD = 2;
int PINCONTROL = 13;
void setup() {
  Serial.begin(9600);   //Mở cổng Serial ở baudrate 9600 để giao tiếp với máy tính
  pinMode(button, INPUT_PULLUP);  //Cài đặt chân D2 ở trạng thái đọc dữ liệu
  pinMode(LED_LCD,OUTPUT);
  pinMode(PINCONTROL,OUTPUT);
  
  digitalWrite(LED_LCD,HIGH);
  digitalWrite(PINCONTROL,HIGH);
}

void loop() {
  int buttonStatus = digitalRead(button);    //Đọc trạng thái button
  Serial.println(buttonStatus);              //Xuất trạng thái button
  if(buttonStatus == 0){
     digitalWrite(LED_LCD,HIGH);
  }else{
     digitalWrite(LED_LCD,LOW);
  }
  delay(200);                                //Chờ 200ms
}
