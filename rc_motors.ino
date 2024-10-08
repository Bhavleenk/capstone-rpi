// Set the size of the arrays (increase for more channels)
#define RC_NUM_CHANNELS 4

// Set up our receiver channels - these are the channels from the receiver
#define RC_CH1  0 // Right Stick LR
#define RC_CH2  1 // Right Stick UD
#define RC_CH3  2 // Left  Stick UD
#define RC_CH4  3 // Left  Stick LR

// Set up our channel pins - these are the pins that we connect to the receiver
#define RC_CH1_INPUT  21 // receiver pin 1
#define RC_CH2_INPUT  20 // receiver pin 2
#define RC_CH3_INPUT  19 // receiver pin 3
#define RC_CH4_INPUT  18 // receiver pin 4

// Set up some arrays to store our pulse starts and widths
uint16_t RC_VALUES[RC_NUM_CHANNELS];
uint32_t RC_START[RC_NUM_CHANNELS];
volatile uint16_t RC_SHARED[RC_NUM_CHANNELS];

// motor LC298 ins
# define in1 4
# define in2 5
# define in3 7
# define in4 6

void setup() {
  
  // Set the speed to communicate with the host PC
  Serial.begin(9600);

  // Set our pin modes to input for the pins connected to the receiver
  pinMode(RC_CH1_INPUT, INPUT);
  pinMode(RC_CH2_INPUT, INPUT);
  pinMode(RC_CH3_INPUT, INPUT);
  pinMode(RC_CH4_INPUT, INPUT);

  // Attach interrupts to our pins
  attachInterrupt(digitalPinToInterrupt(RC_CH1_INPUT), READ_RC1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RC_CH2_INPUT), READ_RC2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RC_CH3_INPUT), READ_RC3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RC_CH4_INPUT), READ_RC4, CHANGE);

  // Motors
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);

  stop_motors();

}
// Thee functions are called by the interrupts. We send them all to the same place to measure the pulse width
void READ_RC1() { 
   Read_Input(RC_CH1, RC_CH1_INPUT); 
}
void READ_RC2() { 
   Read_Input(RC_CH2, RC_CH2_INPUT);
}
void READ_RC3() { 
   Read_Input(RC_CH3, RC_CH3_INPUT); 
}
void READ_RC4() { 
   Read_Input(RC_CH4, RC_CH4_INPUT); 
}
// This function reads the pulse starts and uses the time between rise and fall to set the value for pulse width
void Read_Input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    RC_START[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - RC_START[channel]);
    RC_SHARED[channel] = rc_compare;
  }
}

void forward(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}
void backward(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}
void rotate_clk(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}
void rotate_anti(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}
void stop_motors(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void forward_backward(uint16_t channel_1){
  uint16_t FOR_THRESH = 1600;
  uint16_t BACK_THRESH = 1200;
  uint16_t START_THRESH = 800;
  if (channel_1>FOR_THRESH)
  {  //Forward 
    forward();
    delay(200);
  }
  
  else if (channel_1<BACK_THRESH && channel_1>START_THRESH)
  {  //Backward 
    backward();
    delay(200);
  }
  stop_motors();
}
void rotate(uint16_t channel_2){
  uint16_t START_THRESH = 800;
  uint16_t CLK_THRESH = 1700;
  uint16_t ANTI_THRESH = 1200;
  if (channel_2>CLK_THRESH)
  {  //Clockwise 
    rotate_clk();
    delay(100);
  }
  
  else if (channel_2<ANTI_THRESH && channel_2>START_THRESH)
  {  //Anticlock  
    rotate_anti(); 
    delay(100);
  }
  stop_motors();
}

void loop() {
  
  // read the values from our RC Receiver
  rc_read_values();
  // output our values to the serial port in a format the plotter can use
  forward_backward(RC_VALUES[RC_CH1]);
  rotate(RC_VALUES[RC_CH2]);
  //stop_motors();
  Serial.print(  RC_VALUES[RC_CH1]);  Serial.print(",");
  Serial.print(  RC_VALUES[RC_CH2]);  Serial.print(",");
  Serial.print(  RC_VALUES[RC_CH3]);  Serial.print(",");
  Serial.println(RC_VALUES[RC_CH4]);

}
// this function pulls the current values from our pulse arrays for us to use. 
void rc_read_values() {
  noInterrupts();
  memcpy(RC_VALUES, (const void *)RC_SHARED, sizeof(RC_SHARED));
  interrupts();
}



