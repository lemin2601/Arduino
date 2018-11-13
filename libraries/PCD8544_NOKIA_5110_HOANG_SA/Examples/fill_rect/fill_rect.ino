

#include "PCD8544_HOANGSA.h"
PCD8544 lcd(3,4,5,6);//RST,D/C, Din,CLK
void setup()   {
  lcd.ON();
  lcd.SET(50,0,0,0,4); 
}

void loop(){
  lcd.FillRect(20,10,40,20,BLACK);
  lcd.Display();
}

 // void FillRect(int x,int y, int w, int h, bool color);
 //tô màu hình chữ nhật
 // x,y: tọa độ đỉnh góc trái trên cùng
 // w,h: độ rộng theo trục x, và y
 
