

#include "PCD8544_HOANGSA.h"
PCD8544 lcd(3,4,5,6);//RST,D/C, Din,CLK
void setup()   {
  lcd.ON();
  lcd.SET(50,0,0,0,4); 
}

void loop(){
lcd.FillElip( 42,24, 30,15,BLACK);
lcd.Display();
  
}
//void FillElip(int x0, int y0  , int   xRadius,  int  yRadius, bool color);
//vẽ hình ellipse
// x0,y0: tâm ellipse
// xRadius, yRadius: bán kính trục x và y

