class Cot:public Object
{
  public:
  Cot(unsigned char* t,int x,int y)
  {
    image=t;
    position_x=x;
    position_y=y;
    speed_x=-1;
    heigh=20;
  }
  void drawCot()
  {
    display.drawBitmap(position_x,position_y,image,width,heigh,BLACK);
  }
  void Update()
  {
    if(delaytime%7==0) position_x+=speed_x;
  }
}
