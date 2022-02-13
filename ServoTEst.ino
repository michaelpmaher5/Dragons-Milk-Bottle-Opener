#include <Servo.h>

const int armServo = 5;
int pos = 0;

Servo ArmServo;

void setup() {
  // put your setup code here, to run once:
ArmServo.attach(armServo);
ArmServo.write(pos);
}

void loop() {
  //ArmServo.write(0);
  delay(1000);
  ArmServo.write(75);
  /*for(unsigned int x = 0; x < 90; x += 15)
  {
    ArmServo.write(x);
    delay(2000);
  }
  */
  // put your main code here, to run repeatedly:

  /*
for (unsigned int x = 0; x < 90; x++)
  {
    ArmServo.write(x);
  }
  delay(250);
  for (unsigned int x = 90; x < 180; x++)
  {
    ArmServo.write(x);
  }
  delay(250);
  for (unsigned int x = 180; x < 270; x++)
  {
    ArmServo.write(x);
  }
  delay(250);
  for (unsigned int x = 270; x > 90; x--)
  {
    ArmServo.write(x);
  }
  delay(250);
  */
}
