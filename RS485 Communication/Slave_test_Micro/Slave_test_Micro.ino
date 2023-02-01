#include "timer.h"
#include "Send_Recieve_data.h"

#define Max485_drive 8

// Control Variable
float Measure_speed;
float Motor_speed;
int8_t Motor_directionF;
int8_t Motor_directionB;
float FrontLeft_Angle;
float Set_speed;

//Calculate speed
float  counter = 0;
float  rpm = 0 ;
float  rpmA = 0;
float  rpmB = 0;
float  Target_speed;
float  Set_motor_speed;
float  global_error;
int8_t motor_state;
uint16_t x;

//PID setup
float deltaT = 0.02 ;
float error = 0;
float de_dt = 0;
float e_prev = 0;
float e_integral= 0;
float Output;
float Kp = 3;
float Ki = 2;
float Kd = 0.01;

//PID_2 Setup
float error_2 = 0 ;// e(t-2)
float error_1 = 0 ;// e(t-1)
float error_0 = 0 ; // e(t)
float output = 0 ;  // Usually the current value of the actuator 

//Speed_Measure
bool Flag_50Hz = 0;
bool Flag_16kHz = 0;
uint8_t count_50Hz ;
uint8_t count_16kHz ;
int AB;
float T_timer3 = 0.0000005; //(8/16 MHz)
uint16_t periodA;
uint16_t periodB;
uint16_t ICR1_lastA;
uint16_t ICR1_firstA;
uint16_t ICR1_lastB;
uint16_t ICR1_firstB;
int16_t  counterA = 0;
int16_t  counterB = 0;
float K = 255863.5394;

void setup() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(4, INPUT);
  pinMode(Max485_drive, OUTPUT);
  digitalWrite(Max485_drive, LOW);
  attachInterrupt(digitalPinToInterrupt(2) , Count_pulses_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3) , Count_pulses_B, CHANGE);
  Serial1.begin(1000000);
  //Serial.begin(115200);
  Timer0();
  Timer1();
  Timer3();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if( Flag_50Hz)
  {    
  Flag_50Hz = 0 ;
   FL_Speed += 1;
   
  //Motor_state();
  /*Forward();
  Setservo_angle(recieve_data.FL_Angle);
  Cal_speed();
  PID_controller(recieve_data.FL_Speed);
  Setmotor_speed(Set_motor_speed);*/
  Sending_prepare();
  }
  
  //Serial.println(".");
}
ISR (TIMER0_COMPA_vect)
{
    Sending_data();
}
ISR (TIMER1_OVF_vect) 
{
  if(++count_16kHz == 1){
    count_16kHz = 0;
    Flag_16kHz = 1;
  }
 TCNT1 = 24;
 }
ISR (TIMER3_OVF_vect) 
{
    Flag_50Hz =1;   
}
void Motor_state(){
  motor_state = Motor_directionF + Motor_directionB;
  switch(motor_state){
    case 0:
    Stop();
    break;
    case 1:
    Forward();
    break;
    case 2:
    Backward();
    break;
    case 3:
    Stop();
    break;
    default:
    Stop();
    break;
  }
}
void Setservo_angle(uint8_t Servo_angle){
  OCR3A = mapFloat(Servo_angle, 0, 90, 1000, 3000);
}
void Setmotor_speed(uint16_t Motor_speed ){
  OCR1A = mapFloat(Motor_speed, 0, 400, 23, 1023);
}
void Cal_speed(){
  periodA = ((ICR1_lastA > ICR1_firstA) ? (ICR1_lastA - ICR1_firstA) : (40000 - ICR1_firstA + ICR1_lastA));
  rpmA = (float)(((abs(counterA)-1)*K)/(periodA));
  periodB = ((ICR1_lastB > ICR1_firstB) ? (ICR1_lastB - ICR1_firstB) : (40000 - ICR1_firstB + ICR1_lastB));
  rpmB = (float)(((abs(counterB)-1)*K)/(periodB));
  rpm = (rpmA+rpmB)/2.0;
  Measure_speed = rpm;
  if(Measure_speed >= 400) Measure_speed = 400;
  else if(Measure_speed <= 10) Measure_speed = 0;
  counterA = 0;
  counterB = 0;
}
void Count_pulses_A()
{
  if (counterA) ICR1_lastA = TCNT3;
  else ICR1_firstA = TCNT3;
  AB = PIND & B00000011;                                
  counterA += ((AB != 0) && (AB!= 3) ? (-1) : (+1)); 
}
void Count_pulses_B()
{
  if (counterB) ICR1_lastB = TCNT3;
  else ICR1_firstB = TCNT3;
  AB = PIND & B00000011;                                
  counterB += ((AB != 0) && (AB!= 3) ? (-1) : (+1));
}
void PID_controller(int Tar_speed)
{  
  error = (Tar_speed - Measure_speed) ;
  e_integral += error*deltaT;
  e_integral  = (e_integral > 400)?400:(e_integral < -400)?-400:e_integral;
  de_dt = (error - e_prev)/deltaT;

  Output =  Kp*error + Ki*e_integral + Kd*de_dt;
  Output = (Output > 400)? 400 :(Output < 10) ? 0 : Output ;
  Set_motor_speed = Output;
  global_error = error;
  e_prev = error;
}
void PID_controller_2(int setpoint)
{ 
  float A0 = Kp + Ki*deltaT + Kd/deltaT;
  float A1 = -Kp - 2*Kd/deltaT;
  float A2 = Kd/deltaT;
 
  error_2 = error_1;
  error_1 = error_0;
  error_0 = (setpoint - Measure_speed);
  Output = Output + A0*error_0 + A1*error_1 + A2*error_2;
  Output = (Output > 400)? 400 :(Output < 10) ? 0 : Output;
     
  Set_motor_speed = Output;
}
void Forward()
{
 PORTB |= (1<<PB3);
 PORTB &= ~(1<<PB2);
}
void Backward()
{
 PORTB |= (1<<PB2);
 PORTB &= ~(1<<PB3);
}
void Stop()
{
 PORTB &= ~(1<<PB2);
 PORTB &= ~(1<<PB3);
}
float mapFloat(float x, float x_min, float x_max, float y_min, float y_max)
{
  float y;
  y = y_min + (x - x_min)/(x_max - x_min)*(y_max - y_min);
  y = (y>y_max)?y_max:((y<y_min)?y_min:y);
  return y;
}
