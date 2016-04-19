#include <AFMotor.h>

AF_DCMotor fright(1); //front right wheel is controlled by M1 on SainSmart motor driver shield
AF_DCMotor fleft(2); //front left wheel is controlled by M2 on SainSmart motor driver shield
AF_DCMotor bright(3); //back right wheel is controlled by M3 on SainSmart motor driver shield
AF_DCMotor bleft(4); //back left wheel is controlled by M4 on SainSmart motor driver shield


void setup()
{
  Serial.begin(9600);

  fright.setSpeed(255);
  fright.run(FORWARD);
  delay(2000);
  fleft.setSpeed(255);
  fleft.run(FORWARD);
  delay(2000);
  bright.setSpeed(255);
  bright.run(FORWARD);
  delay(2000);
  bleft.setSpeed(255);
  bleft.run(FORWARD);
  delay(2000);
}

void loop()
{
}
