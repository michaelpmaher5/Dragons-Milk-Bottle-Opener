
#include <SPI.h>
#include <Servo.h>

#define enA A0
#define in1 A1
#define in2 A2
#define enB A5
#define in3 A3
#define in4 A4

#define dirPin 2
#define stepPin 3
#define stepstoopenbottle 1000
#define grasp 75
#define letgo 0
#define set 0
#define moveoutofway 180
#define scissorResetTime 3000

const uint8_t DirPin = 2;
const uint8_t StepPin = 3;
//const uint8_t CSPin = 4;
const int openerServoPin = 5;
const int armServoPin = 6;
const int hingeSwitch = 7;
const int lowerRailSwitchPin = 8;
const int upperRailSwitchPin = 9;
const int buttonPin = 10;
const int bottleSwitch = 11;
const int startPin = 12;
const int indexSwitch = 13;
//const int resetTime = 1000;
int bottlecount = 0;



Servo ArmServo;
Servo OpenerServo;
 
void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(bottleSwitch, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(indexSwitch, INPUT_PULLUP);
  pinMode(hingeSwitch, INPUT_PULLUP);
  ArmServo.attach(armServoPin);
  OpenerServo.attach(openerServoPin);
  pinMode(lowerRailSwitchPin, INPUT_PULLUP);
  pinMode(upperRailSwitchPin, INPUT_PULLUP);
  pinMode(buttonPin,INPUT_PULLUP);
  SPI.begin();

  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  while(digitalRead(startPin) != isPressed(startPin))
  {
    // wait until the start switch is pressed
  }
 // digitalWrite(enablePin, HIGH);
  // if the arm is not currently at the bottom of the rail, move arm until it is
  setDirection(LOW);
  while(digitalRead(lowerRailSwitchPin) != isPressed(lowerRailSwitchPin))
  {
    step();
  }

  // loosen string if not already done so
  ArmServo.write(letgo);
  OpenerServo.write(set);
  delay(1000);
}

void loop() 
{
/////////////////// BEGIN BY GRABBING BOTTLE//////////////////
  ArmServo.write(grasp);
  OpenerServo.write(set);
  setDirection(HIGH);
  delay(500);
  while(digitalRead(hingeSwitch) != isPressed(hingeSwitch))
  {
    step();
  }
  bottlecount++;
  delay(500);
  setDirection(LOW);

  // OPEN THE BOTTLE
  for(int x = 0; x <= stepstoopenbottle; x++)
  {
    step();
  }


 OpenerServo.write(moveoutofway);

 setDirection(HIGH);

 while(digitalRead(upperRailSwitchPin) != isPressed(upperRailSwitchPin))
 {
  step();
 }

 while(digitalRead(buttonPin) != isPressed(buttonPin))
 {
  //loop until the button is pressed
 }
 // Give person the beer
  ArmServo.write(letgo);
  delay(2000); // give time for person to take beer

  // INDEX NEXT BOTTLE AFTER PERSON GRABS BEER

  if (bottlecount == 5)
  {
    while(digitalRead(indexSwitch) != isPressed(indexSwitch))  //move indexing arm back
    {
    clockwise(enB,in3,in4);
    }
    stopmotor(enB,in3,in4);
    bottlecount = 0;
    while(digitalRead(bottleSwitch) != isPressed(bottleSwitch))
    {
      clockwise(enA,in1,in2);
    }
    delay(250);
    counterclockwise(enA,in1,in2);
    delay(scissorResetTime); // move scissor mechanism back
    stopmotor(enA,in1,in2);
  }
  
  while(digitalRead(bottleSwitch) != isPressed(bottleSwitch)) //set up next bottle for arm
  {
    counterclockwise(enB,in3,in4);
  }

  stopmotor(enB,in3,in4);


 setDirection(LOW);
  

  while(digitalRead(lowerRailSwitchPin) != isPressed(lowerRailSwitchPin))  // reset to original position
  {
    step();
  }
  delay(500);
}

bool isPressed(int switchpin) // assumes limit switch is being used with INPUT_PULLUP configuration
{
  int value = digitalRead(switchpin);
  if (value == LOW)
  {
    return true;
  }
  else
  {
    return false;
  } 
}

void step()
{
     digitalWrite(stepPin, HIGH);
     delayMicroseconds(125);
     digitalWrite(stepPin, LOW);
     delayMicroseconds(125);
}

void setDirection(bool dir)   // LOW for down, HIGH for up
{
   digitalWrite(dirPin,dir);
}

void counterclockwise(int enablepin, int pin1, int pin2)
{
  //digitalWrite(enA, LOW);
  //digitalWrite(enB, LOW);
  //delay(50);
  digitalWrite(enablepin, HIGH);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
}

void clockwise(int enablepin,int pin1, int pin2)
{
 // digitalWrite(enA, LOW);
 // digitalWrite(enB, LOW);
  //delay(50);
  digitalWrite(enablepin, HIGH);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void stopmotor(int enablepin, int pin1, int pin2)
{
  digitalWrite(enablepin, LOW);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

// use right hand rule with thumb pointing along axis where direction of fingers is counterclockwise
// when setting enA, set in1 to HIGH and in2 to LOW for counterclockwise, out 4 is red, out 3 is black, for scissor mechanism
// when setting enB, set in3 to HIGH and in4 to LOW for counterclockwise, out 2 is red, out 1 is black, for indexing arm
