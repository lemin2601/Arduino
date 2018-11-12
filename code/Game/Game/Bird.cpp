class Bird:public Object
 
  int frame;
  Bird()
  {
    frame=0;
    position_x=20;
    position_y=20;
    speed_y=1;
    width=16;
    heigh=12;
  }
  void Update() //c?p nh?t v? trí d?i tu?ng
  {
    if(delaytime%5==0)position_y=position_y+speed_y;
    if(position_y>36) position_y=36;
  }
  void Fly()    
  {
    position_y-=speed_y;
  }
  void drawBird()           //V? hình ?nh d?i tu?ng
  {
    if(frame==0)
    {
      display.drawBitmap(position_x,position_y,flappybird,width,heigh,BLACK);
      if(delaytime%25==0){
      frame++;}
    }
    else if(frame==1)
    {
      display.drawBitmap(position_x,position_y,flappybird1,width,heigh,BLACK);
      if(delaytime%25==0){
      frame=0;}
    }
    delaytime++;
  };
