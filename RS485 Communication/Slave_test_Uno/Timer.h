void Timer1(){
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCCR1B |= (1 << WGM13) |(1 << WGM12) | (1 << CS11);
  TCCR1A |= (1 << WGM11) |  (1 << COM1A1)| (1 << COM1B1);
  ICR1 = 100;
  OCR1A = 0;
  OCR1B = 0;
  TIMSK1 = (1 << TOIE1);
  sei();
}
