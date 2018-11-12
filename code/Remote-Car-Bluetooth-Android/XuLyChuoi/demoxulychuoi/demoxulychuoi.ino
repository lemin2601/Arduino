String data;
int led1 = 13; 
int led2 = 13;
void setup() {
  Serial.begin(57600);
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT); 
}
void loop() {
  while (Serial.available()){  //Kiểm tra byte để đọc
  delay(30); //Delay để ổn định hơn 
  char c = Serial.read(); // tiến hành đọc
  if (c == '#') {break;} //Thoát khỏi vòng lặp khi phát hiện từ #
  data += c; // data = data + c
  } 
  if (data.length() > 0) {
    Serial.println(data);  
}
//delay(500);
////Serial.println("agj100Jx20Xy10Y");
//String x = "";
//for(int i =0 ;i<200;i++){
//  x += "sh01Hj100Jy107Yx";//"ah1H
//  x+=i;
//  x+='X';
//  Serial.println(x);
//  x="";
//  delay(250);
//}

} //Thiết lập lại các biến
// Có sự đóng góp và giúp đỡ của Tân_Oggy
