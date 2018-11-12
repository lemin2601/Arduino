//Motor A
int in1 = 7;
int in2 = 6; 
 
//Motor B
int in3 = 5; 
int in4 = 4; 

//set bluetooth
char c;
int led = 13;

 int chuong = 9;
 
void setup()
{
  // set motor
  pinMode(chuong,OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT); 
  
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //set bluetooth
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  
}

 void Motor(char n){
  int speeds =180;
  switch(n){
    case '1': //tien
        //Running motor A
        digitalWrite(in1, HIGH);   
        digitalWrite(in4, HIGH);
        digitalWrite(in2, LOW);  
        digitalWrite(in3, LOW);
    break;
    case '2': //lui
        //Running motor A
        digitalWrite(in1, LOW);   
        digitalWrite(in4, LOW);
        digitalWrite(in2, HIGH);   
        digitalWrite(in3, HIGH);
    break;
    case '3': //trai
        //Running motor A
        digitalWrite(in1, LOW);  
        digitalWrite(in4, HIGH);
        digitalWrite(in2, HIGH);   
        digitalWrite(in3, LOW);
    break;
    case '4': //phai
        //Running motor A
        digitalWrite(in1, HIGH); 
        digitalWrite(in4, LOW);
        digitalWrite(in2, LOW); 
        digitalWrite(in3, HIGH);
    break;
    case '6': //stop
        default:// stop
        digitalWrite(in1, LOW); 
        digitalWrite(in4, LOW);
        digitalWrite(in2, LOW);  
        digitalWrite(in3, LOW);
    break;
    
  }
 }

 
void loop() 
{
//  while(Serial.available())//kiem tra byte de doc
//  {
//    delay(30);// để ổn định hơn
//    
//    c = Serial.read(); 
//    if(c==' ')
//    {
//        digitalWrite(led, LOW); 
//    }
//    else
//    {
//        digitalWrite(led, HIGH); 
//    }
//    Serial.println(c);
//    Motor(c);
//  }

    Motor('1');
    delay(1000);
    
    Motor('2');
    delay(1000);

    Motor('3');
    delay(1000);
    Motor('4');
    delay(1000);
}
