String write_buffer; 
uint8_t write_frame_length; 
bool F_receive = false;
char* p_num; 
char read_buffer[4];      
//-------------------------------------------------------------------------------
void send_num_float(float num_float, char id[2])
{
  Serial.write(id[0]);
  Serial.write(id[1]);
  p_num = (char*)&num_float; 
  Serial.write((char)*(p_num + 3));
  Serial.write((char)*(p_num + 2));
  Serial.write((char)*(p_num + 1));
  Serial.write((char)*p_num);
  Serial.write(';');
}
void send_num_u8(uint8_t num_u8, char id[2])
{
  Serial.write(id[0]);
  Serial.write(id[1]);
  p_num = (char*)&num_u8; 
  Serial.write((char)*p_num);
  Serial.write(';');
}

void send_num_i8(int8_t num_i8, char id[2])
{
  Serial.write(id[0]);
  Serial.write(id[1]);
  p_num = (char*)&num_i8; 
  Serial.write((char)*p_num);
  Serial.write(';'); 
}

void send_num_u16(uint16_t num_u16, char id[2])
{
  Serial.write(id[0]);
  Serial.write(id[1]);
  p_num = (char*)&num_u16; 
  Serial.write((char)*(p_num + 1));
  Serial.write((char)*p_num);
  Serial.write(';');
}

void send_num_i16(int16_t num_i16, char id[2])
{
  Serial.write(id[0]);
  Serial.write(id[1]);
  p_num = (char*)&num_i16; 
  Serial.write((char)*(p_num + 1));
  Serial.write((char)*p_num);
  Serial.write(';');
}

void send_num_u32(uint32_t num_u32, char id[2])
{
  Serial.write(id[0]);
  Serial.write(id[1]);
  p_num = (char*)&num_u32; 
  Serial.write((char)*(p_num + 3));
  Serial.write((char)*(p_num + 2));
  Serial.write((char)*(p_num + 1));
  Serial.write((char)*p_num);
  Serial.write(';');
}

void send_num_i32(int32_t num_i32, char id[2])
{
  Serial.write(id[0]);
  Serial.write(id[1]);
  p_num = (char*)&num_i32; 
  Serial.write((char)*(p_num + 3));
  Serial.write((char)*(p_num + 2));
  Serial.write((char)*(p_num + 1));
  Serial.write((char)*p_num);
  Serial.write(';');
}
