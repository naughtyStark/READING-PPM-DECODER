volatile unsigned long timer[3];
volatile byte last_channel;
volatile int input[8],delT;
volatile byte tick=0;

void setup()
{
  PCICR |= (1 << PCIE0);   
  PCMSK0 |= (1 << PCINT1); //9
  Serial.begin(38400);
}

ISR(PCINT0_vect)
{
  timer[0]=micros();
  //pin 9            
  if(last_channel==0 && PINB & B00000010) //makes sure that the first pin was initially low and is now high
  {                                         //PINB & B00000001 is equivalent to digitalRead but faster
    last_channel = 1;
    delT=timer[0]-timer[1];          
    if(delT>3000)
    {
      tick=0;
    }
    else
    {
      input[tick++] = delT;
    }
    timer[1]=timer[0];
  }
  else if(last_channel==1 && !(PINB & B00000010))
  {
    last_channel=0;
  }
}

void serialprint()
{
  for(int i = 0;i<8;i++)
  {
    Serial.print(input[i]);Serial.print("||");
  }
  Serial.println();
}
  
void loop()
{
  serialprint();
}

