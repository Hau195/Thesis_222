float Measure_speed;
float Motor_speed;
int8_t Motor_directionF;
int8_t Motor_directionB;
float FrontLeft_Angle;
float Set_speed;
float Kp;
float Ki;
float Kd;

char* p_measure_speed = (char*)&Measure_speed;
char* p_motor_speed = (char*)&Motor_speed; 
char* p_motor_directionF = (char*)&Motor_directionF; 
char* p_motor_directionB = (char*)&Motor_directionB;
char* p_FrontLeft_Angle = (char*)&FrontLeft_Angle;
char* p_Kp = (char*)&Kp;
char* p_Ki = (char*)&Ki;
char* p_Kd = (char*)&Kd;
char* p_Set_speed = (char*)&Set_speed;

void Recieved_data(){
  if (Serial.available() >= 7)
  {
    switch (Serial.read())
    {
      case 'S':
      {
        if (Serial.read() == 'P')
          Serial.readBytes(read_buffer, 4); 
        if (Serial.read() == ';')
        {
          *p_motor_speed = (char)read_buffer[3];
          *(p_motor_speed + 1) = (char)read_buffer[2];
          *(p_motor_speed + 2) = (char)read_buffer[1];
          *(p_motor_speed + 3) = (char)read_buffer[0];
        } 
        break;
      }
      case 'R':
      {
        if (Serial.read() == 'W')
          Serial.readBytes(read_buffer, 1); 
        if (Serial.read() == ';')
        {
          *p_motor_directionF = (char)read_buffer[0];
        } 
        break;
      }
      case 'K':
      {
        if (Serial.read() == 'W')
          Serial.readBytes(read_buffer, 1); 
        if (Serial.read() == ';')
        {
          *p_motor_directionB = (char)read_buffer[0];
        } 
        break;
      }
      case 'D':
      {
        if (Serial.read() == 'R')
          Serial.readBytes(read_buffer, 4); 
        if (Serial.read() == ';')
        {
          *p_FrontLeft_Angle = (char)read_buffer[3];
          *(p_FrontLeft_Angle + 1) = (char)read_buffer[2];
          *(p_FrontLeft_Angle + 2) = (char)read_buffer[1];
          *(p_FrontLeft_Angle + 3) = (char)read_buffer[0];
        } 
        break;
      }
        case 'A':
      {
        if (Serial.read() == 'P')
          Serial.readBytes(read_buffer, 4); 
        if (Serial.read() == ';')
        {
          *p_Set_speed = (char)read_buffer[3];
          *(p_Set_speed + 1) = (char)read_buffer[2];
          *(p_Set_speed + 2) = (char)read_buffer[1];
          *(p_Set_speed + 3) = (char)read_buffer[0];
        } 
        break;
      }
          case 'P':
      {
        if (Serial.read() == 'P')
          Serial.readBytes(read_buffer, 4); 
        if (Serial.read() == ';')
        {
          *p_Kp = (char)read_buffer[3];
          *(p_Kp + 1) = (char)read_buffer[2];
          *(p_Kp + 2) = (char)read_buffer[1];
          *(p_Kp + 3) = (char)read_buffer[0];
        } 
        break;
      }
          case 'I':
      {
        if (Serial.read() == 'P')
          Serial.readBytes(read_buffer, 4); 
        if (Serial.read() == ';')
        {
          *p_Ki = (char)read_buffer[3];
          *(p_Ki + 1) = (char)read_buffer[2];
          *(p_Ki + 2) = (char)read_buffer[1];
          *(p_Ki + 3) = (char)read_buffer[0];
        } 
        break;
      }
          case 'T':
      {
        if (Serial.read() == 'P')
          Serial.readBytes(read_buffer, 4); 
        if (Serial.read() == ';')
        {
          *p_Kd = (char)read_buffer[3];
          *(p_Kd + 1) = (char)read_buffer[2];
          *(p_Kd + 2) = (char)read_buffer[1];
          *(p_Kd + 3) = (char)read_buffer[0];
        } 
        break;
      }
    }
  }
}
