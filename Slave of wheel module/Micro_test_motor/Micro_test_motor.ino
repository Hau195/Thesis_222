#include <avr/interrupt.h>
#include "timer.h"
#include "Send_data.h"
#include "Recieve_data.h"

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
int In0_chanelA_read;
int In1_chanelB_read;
float T_timer3 = 0.0000005; //(8/16 MHz)
uint16_t periodA;
uint16_t periodB;
uint16_t ICR1_lastA;
uint16_t ICR1_firstA;
uint16_t ICR1_lastB;
uint16_t ICR1_firstB;
uint16_t lastA;
uint16_t firstA;
uint16_t lastB;
uint16_t firstB;
int16_t  counterA = 0;
int16_t  counterB = 0;
int16_t usable_counterA = 0;
int16_t usable_counterB = 0;
float K = 255863.5394; // 60/(T_timer3*(21.3(tỉ số truyền)*11*2))

void setup() {
  pinMode(16, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(4, INPUT);
  attachInterrupt(digitalPinToInterrupt(2) , Count_pulses_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3) , Count_pulses_B, CHANGE);
  Serial1.begin(115200);
  Serial.begin(115200);
  Timer1();
  Timer3();
}

void loop() {
  if( Flag_50Hz) {   
  Flag_50Hz = 0 ;
  //Motor_state();
  Forward();
  Setservo_angle(FrontLeft_Angle);
  Cal_speed();
  PID_controller(Motor_speed);
  Setmotor_speed(Set_motor_speed);
  send_num_float(Motor_speed,"SC");
  send_num_float(Measure_speed,"ES");
  send_num_float(FrontLeft_Angle, "FL");
  send_num_i8(motor_state, "MS");
  //Serial.print(periodA);
  //Serial.print("=");
  //Serial.print(ICR1_lastA);
  //Serial.print("?");
  //Serial.println(rpm);
  //Serial.print(" : ");
  //Serial.print(O_P);
  //Serial.print(" : ");
  //Serial.println(100);
  }
  Recieved_data();
}
ISR (TIMER1_CAPT_vect) 
{
  uint16_t input_capture = ICR1;
  counter++;
}
ISR (TIMER1_OVF_vect) {
 TCNT1 = 24;
 }
ISR (TIMER3_OVF_vect) {
     Flag_50Hz =1;     
     lastA  =  ICR1_lastA ;
     firstA =  ICR1_firstA;
     lastB  =  ICR1_lastB;
     firstB =  ICR1_firstB;
     usable_counterA = counterA;
     usable_counterB = counterB;
     counterA = 0;
     counterB = 0; 
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
  periodA = ((lastA > firstA) ? (lastA - firstA) : (40000 - firstA + lastA));
  rpmA = (float)(((abs(usable_counterA)-1)*K)/(periodA));
  periodB = ((lastB > firstB) ? (lastB - firstB) : (40000 - firstB + lastB));
  rpmB = (float)(((abs(usable_counterB)-1)*K)/(periodB));
  rpm = (rpmA+rpmB)/2.0;
  Measure_speed = rpm;
  if(Measure_speed >= 400) Measure_speed = 400;
  else if(Measure_speed <= 10) Measure_speed = 0;
}
void Count_pulses_A(){
  if (counterA) ICR1_lastA = TCNT3;
  else ICR1_firstA = TCNT3;
  In0_chanelA_read = PIND & B00000011;                                
  counterA += ((In0_chanelA_read != 0) && (In0_chanelA_read != 3) ? (-1) : (+1)); 
}
void Count_pulses_B(){
  if (counterB) ICR1_lastB = TCNT3;
  else ICR1_firstB = TCNT3;
  In1_chanelB_read = PIND & B00000011;                                
  counterB += ((In1_chanelB_read != 0) && (In1_chanelB_read != 3) ? (-1) : (+1));
}
void PID_controller(int Tar_speed){  
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
void PID_controller_2(int setpoint){ 
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
void Forward(){
 PORTB |= (1<<PB3);
 PORTB &= ~(1<<PB2);
 }
void Backward(){
 PORTB |= (1<<PB2);
 PORTB &= ~(1<<PB3);
 }
void Stop(){
 PORTB &= ~(1<<PB2);
 PORTB &= ~(1<<PB3);
 }
float mapFloat(float x, float x_min, float x_max, float y_min, float y_max){
  float y;
  y = y_min + (x - x_min)/(x_max - x_min)*(y_max - y_min);
  y = (y>y_max)?y_max:((y<y_min)?y_min:y);
  return y;
}
