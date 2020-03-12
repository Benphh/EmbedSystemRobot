#define ISR_PIN_MOTOR1 20 // Define INPUT interrupt pin (p2) 
#define ISR_PIN_MOTOR2 18 // Define INPUT interrupt pin (p2) 

volatile long ticks1 = 0; // Declare global counter variable
volatile long ticks2 = 0; // Declare global counter variable

int motor1_forward = 3; // Motor 1 = Right //3
int motor1_backward = 2; //2

int motor2_forward = 6; //Motor 2 = Left //6
int motor2_backward = 5; //5

byte speed_stopped = 0;
byte speed_slow = 50;
byte master_speed_medium = 100;  
int slave_speed_medium = 100;  
byte speed_vfast = 175; 

int error = 0; 
double kp = 0.13; //This can be changed -DO NOT CHANGE IT FOR THIS ROBOT

void setup() {
  Serial.begin(9600); // I/P sensor connected to p2 requires an internal pullup resistor

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
  forward();
  Serial.print("Motor1: "); //Print counter variable to serial 
  Serial.println(ticks1);
  Serial.print("Motor2: "); //Print counter variable to serial 
  Serial.println(ticks2);
  delay(250);

  error = ticks2 - ticks1;
  slave_speed_medium += error * kp;
  ticks1 = 0;
  ticks2 = 0;

 
  
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
    analogWrite(motor1_forward, slave_speed_medium); 
    analogWrite(motor2_forward, master_speed_medium); 

}
