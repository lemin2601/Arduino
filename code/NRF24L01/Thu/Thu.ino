//Code mẫu nhận chuỗi số và in ra màn hình Serial (Arduino IDE)
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN   9
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN);
int chuoiSo[2];

void setup()
{
  Serial.begin(9600);
  delay(1000);

  Serial.println("Bat dau nhan du lieu");

  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}
void loop()
{
  if (radio.available())
  {
    bool done = false;
    while(!done)
    {
    done = radio.read(chuoiSo, sizeof(chuoiSo));
      digitalWrite(13, HIGH);
      radio.read(chuoiSo, sizeof(chuoiSo) );
      Serial.print("Nhan duoc: ");
      Serial.print(chuoiSo[0]);
      Serial.print("\t");
      Serial.print(chuoiSo[1]);
      Serial.println();
    }
  }
  else
  {
    Serial.println("Khong nhan duoc");
    digitalWrite(13, LOW);
  }
}
