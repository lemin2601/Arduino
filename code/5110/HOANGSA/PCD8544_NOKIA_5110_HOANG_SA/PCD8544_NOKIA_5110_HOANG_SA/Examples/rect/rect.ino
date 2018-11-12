

#include "PCD8544_HOANGSA.h"
PCD8544 lcd(3,4,5,6);//RST,D/C, Din,CLK
void setup()   {
  lcd.ON();
  lcd.SET(50,0,0,0,4); 
}

void loop(){
  lcd.Rect(30,10,30,20,BLACK);
  lcd.Display();
  
}

//void Rect(int x,int y, int w, int h, bool color);
// vẽ hình tứ chữ nhật
//x,y: tọa độ đỉnh góc trái trên cùng
//w,h: chiều dài theo trục ngang và trục dọc
