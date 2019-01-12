/*
                            Project: Line Follower
                            Code Version: 2.0 with PID
                            Platform: Arduino Uno
                            Team: Azmyin Md. Kamal and Raihan Aziz
                            Robot Version: MK-2
                              
///// HARDWARE \\\\\\\
1) 2WD Motor Chassis with caset wheel (Kindly provided my friend Raihan Aziz)
2) L298 Motor Driver Shield (Green)
3) Lipo 3300mAh 25C 3S Battery (I know kinda overkill but it helped me to test the robot many times without worrying about power)
4) Standard 6-12V 200-300RPM motors
5) 5 IR sensor modules (black board with white transmitter bulb)
6) Wires and vero board as necessary
7) Screws and Acryiclic plate as mounting base

////Connection Diagram\\\\\\

////Motor Sheild --> ARDUINO\\\\\

EN1 --> 10 (Left Motor)
EN2 --> 5 (Right Motor)
INA --> 9
INB --> 8
INC --> 7
IND --> 6

////IR SENSOR --> ARDUINO\\\\\\\\
IR 1 --> A0 (Left Sensor)
IR 2 --> A1 (Left Middle Sensor)
IR 3 --> A2 (Middle Sensor)
IR 4 --> A3 (Right Middle Sensor)
IR 5 --> A4 (Right Sensor)

//// MOTOR DIRECTION SCHEME \\\\\\\\\

Left Motor --> INA and INB
Right Motor --> INC and IND

// LIBRARY INCLUSION 
//#include <TimerOne.h>

// Timer1 function APIs
Timer1.restart();
Timer1.start();			
Timer1.stop();

Timer1.initialize(1000000);
Timer1.attachInterrupt(tmrISR); 

void tmrISR()
{
  // Your code to perform here during Timer1 interrupt
}



NOTES
1) In a sunny condition and less than average condition the lowest value
from IR sensor was obtained to be 60 and highest value was 960

Thus we calculate the threshold as such threshold=min_value+(max_value-min_value)/2
For our case the value is 510 i.e value lower than 510 will be taken as zero and value 
over 510 will be taken as

2) Polulo sensor documentation https://www.pololu.com/docs/0J19/all

3) For Line follower usually a parallel PD controller is used. 
To tune Kp at first start with something small and then incresing till 
we find the correct speed. Start with Kp=0.2
*/

#define Kp 0.1
#define Kd 0.0 
#define rightMaxSpeed 150 
#define leftMaxSpeed 150 
#define rightBaseSpeed 55 // Nomial speed when the robot is perfectly straight on the line
#define leftBaseSpeed  55 // Nomial speed when the robot is perfectly straight on the line
#define sampleRate 10




/////////////////////Pin Definitions////////////////////// 

// Right Motor
const int right_pwm=3;
const int inC=4;
const int inD=2;

// Left Motor
const int left_pwm = 5; // Right Motor Enable, this needs to be a PWM pin
const int inA = 6; // Direction controlling variable for right motor
const int inB = 7;

const int LS = A0; 
const int LMS = A1; 
const int MS = A2;
const int RMS = A3;
const int RS = A4;

/////////////////////////////////////////////

/// Function Prototypes \\\\

void IR_read(); // Funciton to read the IR sensor
void thres_check(); // Function to convert the analog readings into discrete value
void callibrate(); // Callibrate to find threshold value from sensor 0, sensor 2
void find_line(); // This function will find an approximate position of the line
// using the weighted average formula from pololu website
void PD_compute(); // Computes the PD response for the current error
// Will also apply special conditions if three sensor reading is encountered
void update_motor(); // Updates the speed of the motor


// Working Variables
int sen[5]; // Variable to store the analog value from each sensor
// in the order from Left Sensor to Right sensor
int x[5]; // This will store either 0 or 1000 for each sensor

int sum=0; // To store the numenator value of the weighted average math operation
// in the find_line() function
int threshold=0;
int line_position=0; // Stores the newest line position
int new_line=0;
int old_line=0;

// PD variables
int error = 0; // Stores current error
int lastError=0; // Stores last error
int ideal_val = 2000; // This is the position value that will be generated when the
// robot's middle sensor is perfectly aligned with the line
int motorSpeed = 0; // motorSpeed = Kp*error + Kd * (error-lastError)

// Motor Variables

int rightMotorSpeed=0;
int leftMotorSpeed=0;

 


void setup()
{
// Set all the motor control pins to outputs
pinMode(right_pwm, OUTPUT);
pinMode(left_pwm, OUTPUT);
pinMode(inA, OUTPUT);
pinMode(inB, OUTPUT);
pinMode(inC, OUTPUT);
pinMode(inD, OUTPUT);
pinMode(13, OUTPUT); // Will use this for callibration phase
 
//Serial.begin(9600);
  
digitalWrite(13, HIGH);  
delay(2000); // Position the sensor
callibrate();
digitalWrite (13,LOW);


for (int i2=0; i2<=7; i2++) // A total halt of 10s
  {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite (13,LOW);
    delay(500);
  
  }

}


void loop()
{
for (int i1=0; i1<=(sampleRate-1); i1++)
  {
    IR_read();
    thres_check();
  }


find_line();
PD_compute();
update_motor(); 
 
sum=0; // Resetting sum
line_position=0; // Resetting line position for next run

} // End of main loop()



