/*
* Chương trình đọc RFID từ RFID-RC522
* Cắm chân (Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SDA: Pin 10
* RST: Pin 9
*/
 
#include <SPI.h>
//#include <RFID.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN);
 
int LEDPin = 3;
 
// Setup variables:
    int serNum0;
    int serNum1;
    int serNum2;
    int serNum3;
    int serNum4;
 
void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}
 
void loop()
{
 
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
                /* Hiển thị số ID của card */
                Serial.println(" ");
                Serial.println("Tim thay RFID Card");
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];
     
                //Serial.println(" ");
                Serial.println("Ma Card:");
                Serial.print("Dec: ");
        Serial.print(rfid.serNum[0],DEC);
                Serial.print(", ");
        Serial.print(rfid.serNum[1],DEC);
                Serial.print(", ");
        Serial.print(rfid.serNum[2],DEC);
                Serial.print(", ");
        Serial.print(rfid.serNum[3],DEC);
                Serial.print(", ");
        Serial.print(rfid.serNum[4],DEC);
                Serial.println(" ");
               
                Serial.print("Hex: ");
        Serial.print(rfid.serNum[0],HEX);
                Serial.print(", ");
        Serial.print(rfid.serNum[1],HEX);
                Serial.print(", ");
        Serial.print(rfid.serNum[2],HEX);
                Serial.print(", ");
        Serial.print(rfid.serNum[3],HEX);
                Serial.print(", ");
        Serial.print(rfid.serNum[4],HEX);
                Serial.println(" ");
                //Bật LED
                analogWrite(LEDPin,30);
                delay(1000);
                analogWrite(LEDPin,0);
          }
    }
 
    rfid.halt();
}

