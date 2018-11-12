#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
Adafruit_PCD8544 display = Adafruit_PCD8544(3,4,5,6,7);
static unsigned char PROGMEM flappybird[]= {0x03, 0xF0, 0x0C, 0x48, 0x10, 0x84, 0x78, 0x8A, 0x84, 0x8A, 0x82, 0x42, 0x82, 0x3E, 0x44, 0x41,0x38, 0xBE, 0x20, 0x41, 0x18,     0x3E, 0x07, 0xC0,};
static unsigned char PROGMEM ca3 [] ={0x0F, 0xC0, 0x12, 0x30, 0x21, 0x09, 0x51, 0x0B, 0x51, 0x07, 0x42, 0x06, 0x7C, 0x97, 0x82, 0x63,0x7D, 0x05, 0x82, 0x08, 0x7C, 0x10, 0x03, 0xE0, };

const int button1=9;
 
const int button2=10;
 
const int button3=11;
 
const int button4=12;
 
const int button5=13;

int buttonTren=0;
 
int buttonPhai=0;
 
int buttonTrai=0;
 
int buttonDuoi=0;
 
int stopGame=0;
bool vaCham(Object &t)   //hàm kiểm tra va chạm
  {
    if((position_x+width>t.position_x)&&(position_x<t.position_x+t.width)
         return true;
    else return false;
  }

  //Game plappy bird 
void flappyBird()
{
  Bird chim;           //khởi tạo đối tượng
  Cot cott1(cottren,84,-10);
  Cot cotd1(cotduoi,84,30);
  Cot cott2(cottren,135,-10);
  Cot cotd2(cotduoi,135,30);
  bool play=true;
  int diem=0;
  while(1)
  {
  if(play)
  {
  display.clearDisplay();
  display.setCursor(70,0); 
  display.print(diem);               //in điểm ra màn hình 
  chim.drawBird();                   //vẽ chim
  chim.Update();                      //cập nhật vị trí chim
  DieuKhien();                         //đọc trang thái điều khiển
  if(buttonTren==HIGH)        //nếu buttonTren được nhấn  gọi hàm fly();
    {
      chim.Fly();    
    }
 
  cott1.drawImage();  //Vẽ cột
  cott2.drawImage();
  cotd1.drawImage();
  cotd2.drawImage();
  cott1.Update();
  cott2.Update();
  cotd1.Update();
  cotd2.Update();
  
  if(cott1.vaCham(chim)||cotd1.vaCham(chim)   //kiểm tra va chạm
  ||cott2.vaCham(chim)||cotd2.vaCham(chim))
  {
    play=false;                                   //nếu va chạm thì gameOver
  }
  if(cott1.GetPositionx()<-9)            // khi đối tượng cột đi hết màn hình cài đặt
  {                                                   // lại vị trí đối tượng
     int tam=random()%21-16;
     int tam1=tam+40;
     cott1.SetPositionx(84);
     cott1.SetPositiony(tam);
     cotd1.SetPositionx(84);
     cotd1.SetPositiony(tam1);
     diem++;
  }
  if(cott2.GetPositionx()<-9)
  {
    int tam2=random()%21-16;
    int tam3=tam2+40;
    cott2.SetPositiony(tam2);
    cotd2.SetPositiony(tam3);
    cotd2.SetPositionx(84);  
    cott2.SetPositionx(84);
    diem++;
  } 
   
  display.display();
  if(stopGame==HIGH)   //ấn nút stop thì dừng game , đưa CT vào 1 vòng lặp
  {
    while(1)
     { 
        DieuKhien(); 
        if(buttonTren==HIGH) break;      //ấn button Trên tiếp tục chơi
        if(buttonDuoi==HIGH) break;      //ấn buttonDuoi quay lại màn hình chính
      }
      if(buttonDuoi==HIGH) break;
  }    
  if(++delaytime>36000) delaytime=0;
  }
   else
   {
     display.clearDisplay();
     display.setCursor(15,10);
     display.print("Game Over");
     display.setCursor(40,20);
     display.print(diem);
     display.display();
     DieuKhien();
     if(buttonTrai==HIGH)     //ấn button Trái tiếp tục chơi
     {
       play=true;
       diem=0;
     }
     if(buttonDuoi==HIGH){break;}   //ấn buttonDuoi quay lại màn hình chính
     cott1.SetPositionx(84);
     cotd1.SetPositionx(84);
     cott2.SetPositionx(126);
     cotd2.SetPositionx(126);    
   }
  }
}
void ChoCaAn()
{
  Fish chuca;                  //Khởi tạo các đối tượng
  ThucAn thucans[10];
  bool Play=true;
  int mang=3;
  int diem=0;
  int lever=25;                 //cấp độ chơi
  for(int i=0;i<10;i++)
     {
       thucans[i].Random();      //khởi tạo ngẩu nhiên vị trị thức ăn
     }
  while(1)
  {
    if(Play)
    {
     display.clearDisplay();
     for(int i=0;i<10;i++)
     {
       thucans[i].Update();        //Cập nhật vị trí
       thucans[i].drawThucAn();   //vẽ đối tượng thức ăn
       if(thucans[i].vaCham(chuca))   // kiểm tra va chạm
      {
        diem++;                                 //nếu va chạm tăng điểm lên 1
        if(diem%100==0) {lever--;if(lever<0)lever=0;thucans[i].SetLever(lever);}
        thucans[i].SetPositiony(0);
        thucans[i].Random();         //khởi tạo lại thức ăn
        chuca.SetAn(21);       
      }
       if(thucans[i].GetPositiony()>=48)
      {
        thucans[i].SetPositiony(0);
        mang--;
        if(mang==0)Play=false;
      } 
     }
     DieuKhien();
     chuca.Update();
     chuca.drawFish();
     
     display.print(diem);
     display.setCursor(65,0);
     for(int i=0;i<mang;i++)
     {display.write(3);}
     display.display();
     
       if(stopGame==HIGH)     //Dừng game
       {
         while(1)
         {  
          DieuKhien(); 
          if(buttonTren==HIGH) break;
          if(buttonDuoi==HIGH) break;
         }
          if(buttonDuoi==HIGH) break;
       }
    
     if(++delaytime>36000) delaytime=0;
    }
    else
    {
      display.clearDisplay();
      display.setCursor(15,10);
      display.print("Game Over");
      display.setCursor(40,20);
      display.print(diem);
      display.display();
      DieuKhien();
      if(buttonTren==HIGH)
      {
        diem=0;
        mang=3;
        for(int i=0;i<10;i++)
        {
          thucans[i].Random();
          thucans[i].SetPositiony(0);
          thucans[i].SetLever(25);
          lever=25;
        }
        Play=true;
      }
      if(buttonDuoi==HIGH){break;}
    }
  }
}
void DieuKhien()
 
  {
 
    buttonTren=digitalRead(button1); //đọc tín hiệu từ cổng 8
 
  //Serial.println(buttonTren);
 
    buttonPhai=digitalRead(button2);
 
    buttonTrai=digitalRead(button3);
 
    buttonDuoi=digitalRead(button4);
 
    stopGame=digitalRead(button5);
 
    Serial.println(stopGame); 
 
  }
void setup() {
  // put your setup code here, to run once:
pinMode(button2,INPUT);
 
pinMode(button3,INPUT);
 
pinMode(button4,INPUT);
 
pinMode(button5,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
