/*
CODE mẫu sử dụng thư viện RF24 để gửi đi 1 chuỗi số tăng dần liên tục.
Nối dây:
   1 - GND to GND
   2 - VCC 3.3V !!! (KHÔNG PHẢI 5V!)
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - Không sử dụng
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// Khai báo chân
#define CE_PIN   9
#define CSN_PIN 10

// Lưu ý: "LL" ở cuối của hằng là kiểu "LongLong"
uint32_t i = 0;
const uint64_t pipe = 0xE8E8F0F0E1LL; // Định nghĩa luồng truyền dữ liệu


RF24 radio(CE_PIN, CSN_PIN);
int chuoiSo[2];  // Mảng chuỗi số 2 phần tử

void setup()
{
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(pipe);
}


void loop()
{
  if(i == 1000) i = 0;
  chuoiSo[0] = i;
  chuoiSo[1] = 1000-i;
  i++;
  radio.write( chuoiSo, sizeof(chuoiSo));
  Serial.println("Da gui ");
  delay(100);
}
