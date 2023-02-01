#include "esp32_timer.h"
#define Max485_drive 27

int counter0 = 0;
bool counter0_flag = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(27, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(Max485_drive, HIGH);
  Serial.begin(115200);
  Serial2.begin(1000000);
  Init_timer0();
  Init_timer1();
  //Init_timer2();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(timer1_flag == 1){
     timer1_flag = 0;
    //FL_Speed = counter0++;
    //if(counter0 > 255) counter0 = 0; 
    Sending_prepare();
    Serial.println(FL_Speed);
    //Serial.println(Serial2.available());
    
  }
  //Serial.println(Serial2.available());
  
  
}
