class ThucAn:public Object
{
  int lever;
  public:
  ThucAn()
  {
    position_x=0;
    position_y=0;
    speed_x=0;
    speed_y=1;
    width=5;
    heigh=5;
    lever=25;
  }
  void drawThucAn()
  {
    display.drawBitmap(position_x,position_y,thucan,width,heigh,BLACK);
  }
  void Random()
  {
    position_x=random()%78;
    speed_y=random()%3+1;
  }
  void SetLever(int i)
  {
    lever=i;
  }
  void Update()
  {
    if(delaytime%lever==0)
    position_y+=speed_y;
  }
};
