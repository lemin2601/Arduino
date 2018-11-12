#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Frame.h"

// pin 8 - Serial clock out (SCLK)
// pin 7 - Serial data out (DIN)
// pin 6 - Data/Command select (D/C)
// pin 9 - LCD reset (RST)
const int button2=10;
const int button1=9;
const int button3=11;
const int button4=12;

int buttonTren=0;
int buttonPhai=0;
int buttonTrai=0;
int buttonDuoi=0;
unsigned int delaytime=0;
int Game=0;
char hoten[]={"Ngo Duc Quang"};
Adafruit_PCD8544 display = Adafruit_PCD8544(3,4,5,6,7);
#include "Object.h"
#include "Cot.h"
#include "Bird.h"
#include "Fish.h"
#include "ThucAn.h"



void setup() 
{
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);
  Serial.begin(9600);
  Serial.println("Here is some math: ");
  display.begin();
  display.setContrast(60);
  display.display(); // show splashscreen
  display.clearDisplay();
  display.drawBitmap(20,2,ITF,44,44,BLACK);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.print("Ngo Duc Quang");
  display.setCursor(0,10);
  display.print("  11T1");
  display.display();
  delay(5000);
  
  
  
  
}
void loop()
{

 display.clearDisplay();
 display.setCursor(0,20);
 display.print("<");
 display.setCursor(79,20);
 display.print(">");
 DieuKhien();
 switch(Game)
 {
   case 0: {
             display.drawBitmap(30,5,flappyicon,24,22,BLACK);
             display.setCursor(10,30);
             display.print("Flappy Bird");
             if(buttonTren==HIGH) flappyBird();
             if(buttonPhai==HIGH) Game=1; 
             break;
           }
   case 1:{
            display.drawBitmap(30,5,caicon,24,22,BLACK);
            display.setCursor(20,30);
            display.print("Nuoi Ca");
            if(buttonTren==HIGH) ChoCaAn();
            if(buttonTrai==HIGH) Game=0; 
            break;
          }
   
 }
 display.display();
    
}


  

void ChoCaAn()
{
  Fish chuca;
  ThucAn thucans[10];
  bool Play=true;
  int mang=3;
  int diem=0;
  int lever=25;
  for(int i=0;i<10;i++)
     {
       thucans[i].Random();
     }
  while(1)
  {
    if(Play)
    {
     display.clearDisplay();
     for(int i=0;i<10;i++)
     {
       thucans[i].Update();
       thucans[i].drawThucAn();
       if(thucans[i].vaCham(chuca))
      {
        diem++;
        if(diem%100==0) {lever--;if(lever<0)lever=0;thucans[i].SetLever(lever);}
        thucans[i].SetPositiony(0);
        thucans[i].Random();
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

//Game plappy bird 
void flappyBird()
{
  Bird chim;
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
  display.print(diem);
  chim.drawBird();
  chim.Update();
  DieuKhien();
  if(buttonTren==HIGH)
    {
      chim.Fly();    
    }
  cott1.drawImage();
  cott2.drawImage();
  cotd1.drawImage();
  cotd2.drawImage();
  cott1.Update();
  cott2.Update();
  cotd1.Update();
  cotd2.Update();
  if(cott1.vaCham(chim)||cotd1.vaCham(chim)
  ||cott2.vaCham(chim)||cotd2.vaCham(chim))
  {
    play=false;
  }
  if(cott1.GetPositionx()<-9)
  {
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
     if(buttonTrai==HIGH)
     {
       play=true;
       diem=0;
     }
     if(buttonDuoi==HIGH){break;}
     cott1.SetPositionx(84);
     cotd1.SetPositionx(84);
     cott2.SetPositionx(126);
     cotd2.SetPositionx(126);
     
   }
  }
}

  void DieuKhien()
  {
    buttonTren=digitalRead(button1);
    Serial.println(buttonTren);
    buttonPhai=digitalRead(button2);
    buttonTrai=digitalRead(button3);
    buttonDuoi=digitalRead(button4);
    
  }

