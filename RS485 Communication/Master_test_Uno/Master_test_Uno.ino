#define Max485_drive 4
#include "Timer.h"
#include "Send_Recieve_data.h"

int counter0 = 0;
bool counter0_flag = 0;

void setup() 
{
  pinMode(Max485_drive, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(Max485_drive, HIGH);
  Serial.begin(1000000);
  Timer1();
}

void loop() {
  // put your main code here, to run repeatedly:
  FL_Angle = 12 ; 
  FL_Speed = counter0++ ;
  if(counter0 > 255) counter0 = 0;
  Sending_prepare();
}
ISR (TIMER1_OVF_vect)
{
    Sending_data();
}
