int32_t input_start[4],input[32],delT[4],reset_timer[4],tick[4]={0,8,16,24};
#define LED PC13

void setup() {
  Serial.begin(57600);
  pinMode(LED,OUTPUT);

  delay(250);
  Timer2.attachCompare1Interrupt(handler_channel_1);//the timer 2 has it's first pin connected to PA0
  Timer2.attachCompare2Interrupt(handler_channel_2);//second pin to PA1
  Timer2.attachCompare3Interrupt(handler_channel_3);//and so on
  Timer2.attachCompare4Interrupt(handler_channel_4);//note that timer 2 has only 4 pre-defined interrupt channels.
  TIMER2_BASE->CR1 = TIMER_CR1_CEN;
  TIMER2_BASE->CR2 = 0;
  TIMER2_BASE->SMCR = 0;
  TIMER2_BASE->DIER = TIMER_DIER_CC1IE | TIMER_DIER_CC2IE | TIMER_DIER_CC3IE | TIMER_DIER_CC4IE;
  TIMER2_BASE->EGR = 0;
  TIMER2_BASE->CCMR1 = 0b100000001; //Register is set like this due to a bug in the define table (30-09-2017)
  TIMER2_BASE->CCMR2 = 0b100000001; //Register is set like this due to a bug in the define table (30-09-2017)
  TIMER2_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
  TIMER2_BASE->PSC = 127;
  TIMER2_BASE->ARR = 0xFFFF;
  TIMER2_BASE->DCR = 0;
}

bool blink = 0;

void loop() {
  delay(50);
  digitalWrite(LED,blink);
  blink = !blink;
  for(int i =0;i<4;i++)
  {
    Serial.print(reset_timer[i]);Serial.print("||");
    
  }
  Serial.println();
}



void handler_channel_1(void)
{
  if (0b1 & GPIOA_BASE->IDR  >> 0) //check if pin PA0 is high, the GPIOA_BASE refers to port A, IDR refers to interrupt detect register which contains the pin number
  {                                // of the pin that just went high and >> 0 is us shifting the IDR by 0 bits. if the IDR's first bit is 1, 0b1&1 would give 1, this means
                                   //that there was a rising edge (the CCER is set to detect the rising edge at first)
    delT[0] = TIMER2_BASE->CCR1 - input_start[0];
    if (delT[0]< 0)delT[0]+= 0xFFFF;
    if(reset_timer[0]>2000)
    {
      tick[0] = 0;
    }
    else
    {
      input[tick[0]++] = delT[0];
    }
    input_start[0] = TIMER2_BASE->CCR1;//the time of the rising edge is stored in input_start[0] variable
    TIMER2_BASE->CCER |= TIMER_CCER_CC1P;// the Compare capture enable register (CCER) is set to detect the falling edge on pin PA0 (CC1P-> a binary value 
  }                                       // in which the value corresponding to pin 1(PA0) is set to high (how do i know its set to high? look at the |= ).
  else    //when the interrupt is generated BUT the edge is not the rising edge but rather the falling edge
  {
    reset_timer[0] = TIMER2_BASE->CCR1 - input_start[0];//the pulse time is the falling edge time - rising edge time.
    if (reset_timer[0]< 0)reset_timer[0]+= 0xFFFF; //if for some reason the value of the channel is less than 0(due to timer restart) then just take the complement.
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC1P; //reset the CCER to detect the rising edge by taking bitwise AND with the compliment of CC1P(doesn't touch anything else but resests
  }                                         //the bit we care about.
}

void handler_channel_2(void)
{
  if (0b1 & GPIOA_BASE->IDR >> 1) 
  {                                // of the pin that just went high and >> 0 is us shifting the IDR by 0 bits. if the IDR's first bit is 1, 0b1&1 would give 1, this means                                //that there was a rising edge (the CCER is set to detect the rising edge at first)
    delT[1] = TIMER2_BASE->CCR2 - input_start[1];
    if (delT[1]< 0)delT[1]+= 0xFFFF;
    if(reset_timer[1]>2000)
    {
      tick[1] = 8;
    }
    else
    {
      input[tick[1]++] = delT[1];
    }
    input_start[1] = TIMER2_BASE->CCR2;
    TIMER2_BASE->CCER |= TIMER_CCER_CC2P;
  }
  else
  {
    reset_timer[1]= TIMER2_BASE->CCR2 - input_start[1];
    if (reset_timer[1]< 0)reset_timer[1]+= 0xFFFF;
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC2P;
  }
}

void handler_channel_3(void)
{
  if (0b1 & GPIOA_BASE->IDR >> 2) 
  {                                // of the pin that just went high and >> 0 is us shifting the IDR by 0 bits. if the IDR's first bit is 1, 0b1&1 would give 1, this means
                                   //that there was a rising edge (the CCER is set to detect the rising edge at first)
    delT[2] = TIMER2_BASE->CCR3 - input_start[2];
    if (delT[2]< 0)delT[2]+= 0xFFFF;
    if(reset_timer[2]>2000)
    {
      tick[2] = 16;
    }
    else
    {
      input[tick[2]++] = delT[2];
    }
    input_start[2] = TIMER2_BASE->CCR3;
    TIMER2_BASE->CCER |= TIMER_CCER_CC3P;
  }
  else
  {
    reset_timer[2]= TIMER2_BASE->CCR3 - input_start[2];
    if (reset_timer[2]< 0)reset_timer[2]+= 0xFFFF;
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC3P;
  }
}

void handler_channel_4(void)
{
  if (0b1 & GPIOA_BASE->IDR >> 3) 
  {                                // of the pin that just went high and >> 0 is us shifting the IDR by 0 bits. if the IDR's first bit is 1, 0b1&1 would give 1, this means
                                  //that there was a rising edge (the CCER is set to detect the rising edge at first)
    delT[3] = TIMER2_BASE->CCR4 - input_start[3];
    if (delT[3]< 0)delT[3]+= 0xFFFF;
    if(reset_timer[3]>2000)
    {
      tick[3] = 24;
    }
    else
    {
      input[tick[3]++] = delT[3];
    }
    input_start[3] = TIMER2_BASE->CCR4;
    TIMER2_BASE->CCER |= TIMER_CCER_CC4P;
  }
  else 
  {
    reset_timer[3]= TIMER2_BASE->CCR4 - input_start[3];
    if (reset_timer[3]< 0)reset_timer[3]+= 0xFFFF;
    TIMER2_BASE->CCER &= ~TIMER_CCER_CC4P;
  }
}


