#include "Send_Recieve_data.h"
bool timer1_flag = 0;

hw_timer_t * timer0 = NULL;
portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t * timer1 = NULL;
portMUX_TYPE timer1Mux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t * timer2 = NULL;
portMUX_TYPE timer2Mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR ontimer0()
{     
  portENTER_CRITICAL_ISR(&timer0Mux);    
  Sending_data();
  portEXIT_CRITICAL_ISR(&timer0Mux);  
} 
void IRAM_ATTR ontimer1()
{     
  portENTER_CRITICAL_ISR(&timer1Mux);    
  timer1_flag = 1;
  portEXIT_CRITICAL_ISR(&timer1Mux);  
} 
/*void IRAM_ATTR ontimer2()
{     
  portENTER_CRITICAL_ISR(&timer2Mux);    
  portEXIT_CRITICAL_ISR(&timer2Mux);  
} */

void Init_timer0()
{
  timer0 = timerBegin(0, 40, true); 
  timerAttachInterrupt(timer0, &ontimer0, true);
  timerAlarmWrite(timer0, 100, true);
  timerAlarmEnable(timer0);
}
void Init_timer1()
{
  timer1 = timerBegin(1, 40, true); 
  timerAttachInterrupt(timer1, &ontimer1, true);
  timerAlarmWrite(timer1, 40000, true);
  timerAlarmEnable(timer1);
}
/*void Init_timer2()
{
  timer2 = timerBegin(2, 40, true); 
  timerAttachInterrupt(timer2, &ontimer2, true);
  timerAlarmWrite(timer2, 40000, true);
  timerAlarmEnable(timer2);
}*/
