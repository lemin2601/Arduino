const int trig = 7;     // chân trig của HC-SR04
const int echo = 8;     // chân echo của HC-SR04
volatile   int distance =0;  
void setup()
{
    Serial.begin(9600);     // giao tiếp Serial với baudrate 9600
    pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
    pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu\
    
}
 
void getDistance()
{  
    detachInterrupt(0);
    unsigned long duration; // biến đo thời gian
//    int distance;           // biến lưu khoảng cách
    
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(10);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
   
}
void loop(){
    /* In kết quả ra Serial Monitor */
    Serial.print("cai quai j day");
 getDistance();
    Serial.print(distance);
    Serial.println("cm");
    delay(200);
}
