#define ISR_PIN_MOTOR1 20 // Define INPUT interrupt pin (p2) 
#define ISR_PIN_MOTOR2 18 // Define INPUT interrupt pin (p2) 

//***LEFT SENSOR VALUES***//
int left_trigger_pin = 12;
int left_echo_pin = 13;
int left_vcc_pin = 11;

int left_time;
int left_distance;

//***FORWARD SENSOR VALUES***//
int forward_trigger_pin = 9;
int forward_echo_pin = 10;
int forward_vcc_pin = 8;

int forward_time;
int forward_distance;

volatile long ticks1 = 0; // Declare global counter variable
volatile long ticks2 = 0; // Declare global counter variable

int motor1_forward = 3; // Motor 1 = Right
int motor1_backward = 2;

int motor2_forward = 6; //Motor 2 = Left 
int motor2_backward = 5;

int error = 0; 
double kp = 0.1; //This can be changed

int rightAngleTicks = 2150;
int halfRightAngleTicks = 1075;

int forward_limit = 20;
int num_turns = 0;

byte speed_stopped = 0;
byte speed_slow = 50;
byte speed_medium = 60;
byte speed_rmedium = 66;  
byte speed_vfast = 175;  

void setup() {
  Serial.begin(9600); // I/P sensor connected to p2 requires an internal pullup resistor
   pinMode(left_vcc_pin, OUTPUT);
  pinMode (left_trigger_pin, OUTPUT);
  pinMode (left_echo_pin, INPUT);

  pinMode(forward_vcc_pin, OUTPUT);
  pinMode (forward_trigger_pin, OUTPUT);
  pinMode (forward_echo_pin, INPUT);
  //Declaring Motor pins as outputs
  pinMode(motor1_forward, OUTPUT); 
  pinMode(motor1_backward, OUTPUT);
  pinMode(motor2_forward, OUTPUT);
  pinMode(motor2_backward, OUTPUT);

  //Setup for Encoder for Motor 1
  pinMode(ISR_PIN_MOTOR1 , INPUT_PULLUP); // Attach the ISR (countPULSE) to any change on pin (p2)
  attachInterrupt(digitalPinToInterrupt(ISR_PIN_MOTOR1),countPULSE1,CHANGE); 

  //Setup for Encoder for Motor 2
  pinMode(ISR_PIN_MOTOR2 , INPUT_PULLUP); // Attach the ISR (countPULSE) to any change on pin (p2)
  attachInterrupt(digitalPinToInterrupt(ISR_PIN_MOTOR2),countPULSE2,CHANGE); 
 }


void loop() {
  //Forward Ultrasonic Sensor Code
  digitalWrite(forward_vcc_pin, HIGH);
  digitalWrite(forward_vcc_pin, HIGH);
  digitalWrite (forward_trigger_pin, HIGH);
  delayMicroseconds (10);
  digitalWrite (forward_trigger_pin, LOW);
  forward_time = pulseIn (forward_echo_pin, HIGH);
  forward_distance = (forward_time * 0.034) / 2;
  if(forward_distance<0){
    forward_distance = 80;
  }
  digitalWrite (left_trigger_pin, HIGH);
  delayMicroseconds (10);
  digitalWrite (left_trigger_pin, LOW);
  left_time = pulseIn (left_echo_pin, HIGH);
  left_distance = (left_time * 0.034) / 2;
  if(left_distance<0){
    left_distance = 80;
  }
  forward();
  delay(100);


  if(forward_distance < forward_limit){
    stopped();
    delay(50);
    ticks1 = 0;
    ticks2 = 0;
    rightTurn();
    num_turns++;
    
  }else{
    forward();
    delay(1000);
    ticks1 = 0;
    ticks2 = 0;
    if(left_distance<15){
      stopped();
      delay(50);
      right();
      delay(70);
      stopped();
      delay(50);
    }
    if(left_distance>30){
      stopped();
      delay(50);
      left();
      delay(95);
      stopped();
      delay(50);
    }
    
    if(num_turns==6){
      halfRightTurn();
  }
  }

  
  
  
}

void countPULSE1() {// **** Interrupt Service Routine (ISR) **** 
  ticks1++; // Increment global pulse counter 
}
void countPULSE2() {// **** Interrupt Service Routine (ISR) **** 
  ticks2++; // Increment global pulse counter 
}

void forward(){
    Serial.println("Forwards");
    
    analogWrite(motor1_backward, speed_stopped); 
    analogWrite(motor2_backward, speed_stopped); 
    analogWrite(motor1_forward, speed_rmedium); 
    analogWrite(motor2_forward, speed_medium); 

}
void backward(){
    Serial.println("backwards");
    
    analogWrite(motor1_forward, speed_stopped); 
    analogWrite(motor2_forward, speed_stopped); 
    analogWrite(motor1_backward, speed_medium); 
    analogWrite(motor2_backward, speed_medium); 
}

void left(){
    Serial.println("right");
    
    analogWrite(motor1_forward, speed_medium); 
    analogWrite(motor2_forward, speed_stopped); 
    analogWrite(motor1_backward, speed_stopped); 
    analogWrite(motor2_backward, speed_stopped); 
}


void right(){
    Serial.println("left");
    
    analogWrite(motor1_forward, speed_stopped); 
    analogWrite(motor2_forward, speed_medium); 
    analogWrite(motor1_backward, speed_stopped); 
    analogWrite(motor2_backward, speed_stopped); 
}

void stopped(){
    Serial.println("Stopped");
    
    analogWrite(motor1_forward, speed_stopped); 
    analogWrite(motor2_forward, speed_stopped); 
    analogWrite(motor1_backward, speed_stopped); 
    analogWrite(motor2_backward, speed_stopped); 
}

void rightTurn(){
  while(ticks2<rightAngleTicks){
    right();
  }
  stopped();
}

void halfRightTurn(){
  while(ticks2<halfRightAngleTicks){
    right();
  }
  forward();
  delay(1000);
  stopped();
  delay(20000);
  
}
