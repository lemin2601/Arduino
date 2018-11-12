// encoder xung 
#define encoderTrai 18
#define encoderPhai 19
#define analogTrai A4
#define analogPhai A5
volatile unsigned int encoderPosTrai =0;
volatile unsigned int encoderPosPhai =0;
unsigned int lastReportedPosTrai =1;
unsigned int lastReportedPosPhai =1;
static boolean rotating = false; // chống nhiễu

// biến phục vụ ngắt
boolean A_set = false;
boolean B_set = false;

unsigned long oldtime =0;
unsigned long newtime =0;
float vantocTrai;
float vantocPhai;
int pusle  =0 ;

    
void setup() {
  // put your setup code here, to run once:

 // Set đọc vận tốc
  pinMode(encoderTrai,INPUT_PULLUP);
  pinMode(encoderPhai,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderTrai), doEncoderTrai, CHANGE);
  //attachInterrupt(0 ,doEncoderTrai,CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPhai) ,doEncoderPhai,CHANGE);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  newtime = millis();
  if(newtime - oldtime >= 1000){
    //tinh van toc
      vantocTrai = (encoderPosTrai)*1000/((newtime - oldtime)*40); 
      vantocPhai = (encoderPosPhai)*1000/((newtime - oldtime)*40);
     Serial.print("Trai :");      Serial.println(encoderPosTrai); Serial.print("Phai :");      Serial.println(encoderPosPhai);
     encoderPosTrai =0; encoderPosPhai =0;
     // lcd.setCursor(0,0); lcd.print("ARFUINO");     
      //lcd.setCursor(1,0);lcd.print("DoiMa");
    oldtime = newtime;
  }
}
void doEncoderTrai(){
  delay(1);
  if(digitalRead(encoderTrai) != A_set){
    A_set = !A_set;
    encoderPosTrai += 1;
  }
}
void doEncoderPhai(){
   delay(1);
   if(digitalRead(encoderPhai) != B_set){
    B_set = !B_set;
    encoderPosPhai += 1;
  }
}
