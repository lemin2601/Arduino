class Fish:public Object
{
 
  bool Phai;
  int An;
  public:
 
  Fish()
  {
    position_x=20;
    position_y=20;
    speed_x=1;
    speed_y=1;
    width=16;
    heigh=12;
    Phai=true;
    An=0;
  }
  void drawFish()
  {
    if(buttonPhai) Phai=true;
    if(buttonTrai) Phai=false;
    if(Phai)
    {
      if(!An)
      display.drawBitmap(position_x,position_y,ca,width,heigh,BLACK);
      else display.drawBitmap(position_x,position_y,ca1,width,heigh,BLACK);
    }
 
    else
    {
      if(!An)
      display.drawBitmap(position_x,position_y,ca3,width,heigh,BLACK);
      else display.drawBitmap(position_x,position_y,ca2,width,heigh,BLACK);
    }
    An--;
    if(An<0) An=0;
  }
 
  void SetAn(int i)
  {
    An=i;
  }
  void Update()
  {
    if(buttonTren) position_y-=speed_y;
    if(buttonDuoi) position_y+=speed_y;
    if(buttonPhai) position_x+=speed_x;
    if(buttonTrai) position_x-=speed_x;
    if(position_y>36) position_y=36;
    if(position_y<0) position_y=0;
    if(position_x>83) position_x=-15;
    if(position_x<-15) position_x=83;
  } 
}
