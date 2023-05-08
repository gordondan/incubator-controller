#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();   // Initiates library.

#define SERVOMIN  100  // Minimum pulse length count out of 4096.
//#define SERVOMAX  2200 // Maximum pulse length count out of 4096.
//#define SERVOMAX  275 // Maximum pulse length count out of 4096.
#define SERVOMAX  565 // Maximum pulse length count out of 4096.
#define EGGSLEFT  216
#define EGGSMIDDLE 332
#define EGGSRIGHT 449
#define DELAYPERINCRMENT 75
#define INCREMENTSTEP 1
int servoNo = 15; // Defines a counter for servos.


void setup() 
{
  Serial.begin(9600);       // Starts serial connecton at 9600 baud rate.
  
  pwm.begin();         // Sends PWM signals.
  pwm.setPWMFreq(60);  // Makes servos run at 60 Hz rate.
  delay(20);
  pwm.setPWM(15, 0, EGGSLEFT);
}


void loop() 
{   
  //pwm.setPWM(15, 0, SERVOMIN);
  //delay(2000);   
  //pwm.setPWM(15, 0, SERVOMAX);
  //delay(2000);
//pwm.setPWM(15, 0, EGGSLEFT);
//  delay(2000);
//pwm.setPWM(15, 0, EGGSMIDDLE);
//  delay(2000);
//pwm.setPWM(15, 0, EGGSRIGHT);
//  delay(2000);
moveEggsRight();
waitOneFifthOfAday();
moveEggsLeft();
waitOneFifthOfAday();
} 

void moveEggsLeft(){
  for (int pulselen = EGGSRIGHT; pulselen > EGGSLEFT; pulselen = pulselen-INCREMENTSTEP){
    pwm.setPWM(servoNo, 0, pulselen);
    delay(DELAYPERINCRMENT);
  }
}

void moveEggsRight(){
  for (int pulselen = EGGSLEFT; pulselen < EGGSRIGHT; pulselen = pulselen+INCREMENTSTEP){
    pwm.setPWM(servoNo, 0, pulselen);
    delay(DELAYPERINCRMENT);
  }
}

void waitOneFifthOfAday(){
  delay(17280000);
}
