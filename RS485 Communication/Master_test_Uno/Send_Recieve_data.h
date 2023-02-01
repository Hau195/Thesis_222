
byte Frame_header = 36;
byte Modular_ID = 1;
byte Sending_order = 10;
byte Frame_footer = 35;

int send_status = 0;
int counter1 = 0;
bool counter1_flag = 0;
int counter2 = 0;
bool counter2_flag = 0;
int data;
int buffer_RX_index = 0;
int Header_index;
int Footer_index;
uint8_t Frame_position = 0;

float FL_Angle = 0;
float FL_Speed = 0;
float FR_Angle = 0;
float FR_Speed = 0;
float RL_Angle = 0;
float RL_Speed = 0;
float RR_Angle = 0;
float RR_Speed = 0;  

bool done_copy_flag = 0;
bool can_copy_flag = 0;
bool header_flag = 0;
bool footer_flag = 0;
bool flag = 1 ;
bool send_flag = 0 ;
bool recieve_flag = 0 ;
byte buffer_read[40];
byte buffer_receive[1];
byte buffer_RX[80];

struct Data_send{
      byte Header[4] = {Frame_header,40,0,0};
      float FL_Angle = 10;
      float FL_Speed = 120;
      float FR_Angle = 10;
      float FR_Speed = 120;
      float RL_Angle = 10;
      float RL_Speed = 120;
      float RR_Angle = 10;
      float RR_Speed = 120;
      byte Footer[4] = {0,Sending_order,Modular_ID,Frame_footer};
}send_data;

struct Data_recieved{
      byte Header[4] = {0,0,0,0};
      float FL_Angle;
      float FL_Speed;
      float FR_Angle;
      float FR_Speed;
      float RL_Angle;
      float RL_Speed;
      float RR_Angle;
      float RR_Speed;
      byte Footer[4] = {0,0,0,0};
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


void Receiving_data()
{
   if(Serial.available())
     {
      Serial.readBytes(buffer_receive, 1);
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
         memcpy(&recieve_data, buffer_read, 40);
         if(buffer_read[38] == 2)
         {
          FL_Angle = recieve_data.FL_Angle; 
          FL_Speed = recieve_data.FL_Speed;

          buffer_RX_index = 0;
          Frame_position = 0;
         }
         else if(buffer_read[38] == 3)
         {
          FR_Angle = recieve_data.FR_Angle; 
          FR_Speed = recieve_data.FR_Speed;
         
          buffer_RX_index = 0;
          Frame_position = 0;
         }
         else if(buffer_read[38] == 4)
         {
          RL_Angle = recieve_data.RL_Angle; 
          RL_Speed = recieve_data.RL_Speed;
          
          buffer_RX_index = 0;
          Frame_position = 0;
         }
         else if(buffer_read[38] == 5)
         {
          RR_Angle = recieve_data.RR_Angle; 
          RR_Speed = recieve_data.RR_Speed;
          
          buffer_RX_index = 0;
          Frame_position = 0;
         }
         if(buffer_read[37] == 11)
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
              PORTG |= (1<<PG5);        
              Serial.write((byte*)&send_data, sizeof(send_data));
              send_status = 1;
            } 
              break;    
     case 1:   
          if(Serial.availableForWrite() == 63)
            {
              send_status = 2;
              counter2 = 1;
            }
             break;     
    case 2:   
          if(counter2-- == 0)
            {
              PORTG &= ~(1<<PG5);
              send_status = 3;
            }
             break;
    case 3:
              Receiving_data();
             break;
    case 4: 
            if(send_flag == 1)
            { 
              send_flag = 0; 
              send_status = 0;
              counter1 = 0;
            }
             break;
     }
}

void Sending_prepare()
{
  send_data.FL_Angle = FL_Angle; 
  send_data.FL_Speed = FL_Speed;
  //send_data.FR_Angle = FR_Angle; 
  //send_data.FR_Speed = FR_Speed;
  //send_data.RL_Angle = RL_Angle; 
  //send_data.RL_Speed = RL_Speed;
  //send_data.RR_Angle = RR_Angle; 
  //send_data.RR_Speed = RR_Speed;
  send_flag = 1;
}
  
