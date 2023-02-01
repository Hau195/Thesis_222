
//Send ID
byte Frame_header = 36;
byte Modular_ID = 2;
byte Sending_order = 11;
byte Frame_footer = 35;

//
int counter1 = 2;
int counter2 = 2;
int send_status = 3;
int buffer_RX_index = 0;
int Header_index;
int Footer_index;
uint8_t Frame_position = 0;

//Variable for controlling wheel modular
float FL_Angle = 0;
float FL_Speed = 0;

//name flag
bool counter1_flag = 0;
bool counter2_flag = 0;
bool done_copy_flag = 0;
bool can_copy_flag = 0;
bool header_flag = 0;
bool footer_flag = 0;
bool send_flag = 0 ;
bool recieve_flag = 0 ;

//name data
int16_t data;
byte buffer_read[40];
byte buffer_receive[1];
byte buffer_RX[80];

struct Data_send{
      byte Header[4] = {Frame_header,40,0,0};
      float FL_Angle = 40;
      float FL_Speed = 118;
      float FR_Angle = 0;
      float FR_Speed = 0;
      float RL_Angle = 0;
      float RL_Speed = 0;
      float RR_Angle = 0;
      float RR_Speed = 0;
      byte Footer[4] = {0,Sending_order,Modular_ID,Frame_footer};
}send_data;
struct Data_recieved{
      byte Header[4] = {0, 0, 0, 0};
      float FL_Angle;
      float FL_Speed;
      float FR_Angle;
      float FR_Speed;
      float RL_Angle;
      float RL_Speed;
      float RR_Angle;
      float RR_Speed;
      byte Footer[4] = {0, 0, 0, 0};
}recieve_data;


void copy_array(int Header_index, int Footer_index)
{
  int bufer_read_index = 0;
  for(int i = Header_index; i<= Footer_index; i++)
  {
    buffer_read[bufer_read_index] = buffer_RX[i];
    bufer_read_index ++;
  }
}
void Find_H_F()
{
  switch(Frame_position){
    case 0:
      if(buffer_receive[0] == 36)
      { 
        Header_index = buffer_RX_index;
        Frame_position = 1;
      }
      break;
    case 1:
      if(buffer_receive[0] == 35)
      {
        Footer_index = buffer_RX_index;
        Frame_position = 0;
        can_copy_flag = 1;
      }
      break;
  }
}

void Sending_prepare()
{
  send_data.FL_Angle = FL_Angle; 
  send_data.FL_Speed = FL_Speed;
  send_flag = 1;
}

void Receiving_data()
{
   if(Serial1.available())
     {
      Serial1.readBytes(buffer_receive,1);
      Find_H_F();
      buffer_RX[buffer_RX_index++] = buffer_receive[0];
      if(can_copy_flag == 1)
        {
          can_copy_flag = 0;
          copy_array(Header_index, Footer_index);
          done_copy_flag = 1;
        }
      if(buffer_RX_index >= 80)
        {
          buffer_RX_index = 0;
          Frame_position = 0;
        }
      if(done_copy_flag == 1)
        {
         done_copy_flag = 0;
         if(buffer_read[38] == 1)
         {
          memcpy(&recieve_data, buffer_read, 40);
          FL_Angle = recieve_data.FL_Angle; 
          FL_Speed = recieve_data.FL_Speed;
          
          buffer_RX_index = 0;
          Frame_position = 0;
         }
         if(buffer_read[37] == 10)
         {
          send_status = 4;
          
         }
        }
     } 
}
void Sending_data()
{
switch(send_status)
  {
    case 0:
          if(counter1-- == 0)
          {   
            digitalWrite(6,HIGH);   
            PORTB |= (1<<PB4);              
            Serial1.write((byte*)&send_data, sizeof(send_data));
            send_status = 1;
          }
           break;
    case 1:   
          if(Serial1.availableForWrite() == 63)
          {
            send_status = 2;
            counter2 = 1;
          }
          break;   
    case 2:   
          if(counter2-- == 0)
          {
            PORTB &= ~(1<<PB4);
            send_status = 3;
          }
          break;
    case 3:   
          Receiving_data();
          break;
    case 4:  
          if(send_flag)
          { 
          send_flag = 0; 
          send_status = 0;
          counter1 = 0;
          }
          break; 
  }
}
