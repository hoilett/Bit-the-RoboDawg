#include <AFMotor.h> //Motor library
#include <nRF24L01.h> //Radio libraries
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

<<<<<<< HEAD
AF_DCMotor fright(1); //front right wheel is controlled by M1 on SainSmart motor driver shield
AF_DCMotor fleft(2); //front left wheel is controlled by M2 on SainSmart motor driver shield
AF_DCMotor bright(3); //back right wheel is controlled by M3 on SainSmart motor driver shield
AF_DCMotor bleft(4); //back left wheel is controlled by M4 on SainSmart motor driver shield
=======
//MOTOR DRIVERS
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
>>>>>>> 310cb0fae3a9a1e80fc5d33b61063bde8a0b6c3f

//ENVIRONMENT VARIABLES
int radioBoolean = false;
int messageLength = 12;
int msg[1];
RF24radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";

void setup()
{
  Serial.begin(9600);
<<<<<<< HEAD

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
=======
  Serial.println("Serial started");
  radio.begin();
  Serial.println("Radio started");
  radio.openReadingPipe(1,pipe);
  Serial.println("Pipe opened");
  radio.StartListening();
  Serial.println("LIstening...");
>>>>>>> 310cb0fae3a9a1e80fc5d33b61063bde8a0b6c3f
}

void loop()
{
  receiveRadio();
    
}

void receiveRadio()
{
    if (radio.available)
    {
      bool done = false;
      done = radio.read(msg, 1);
      char theChar = 
    }
    else
    {
      Serial.println("No radio signal");
    }
}
