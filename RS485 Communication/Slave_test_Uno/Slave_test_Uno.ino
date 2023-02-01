#define Max485_drive 4
#include "Timer.h"
#include "Send_Recieve_data.h"

int counter0 = 0;
bool counter0_flag = 0;

void setup() 
{
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(Max485_drive, LOW);
  Serial.begin(1000000);
  Timer1();
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(13, FL_Speed);
  Sending_prepare();
}

ISR (TIMER1_OVF_vect)
{
  Sending_data(); 
}
