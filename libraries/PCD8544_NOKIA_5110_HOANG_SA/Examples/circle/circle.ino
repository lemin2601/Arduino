

#include "PCD8544_HOANGSA.h"
PCD8544 lcd(3,4,5,6);//RST,D/C, Din,CLK


void setup()   {

  lcd.ON();
  lcd.SET(50,0,0,0,4);
  
}


void loop(){
  lcd.Circle( 42,24,20,BLACK);
  lcd.Display();
}

  // void Circle(int x0,int y0,int r, bool color);
  // x0,y0: tọa độ tâm
  // r: bán kính
