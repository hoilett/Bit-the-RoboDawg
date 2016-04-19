#include <AFMotor.h> //Motor library
#include <nRF24L01.h> //Radio libraries
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

//MOTOR DRIVERS
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

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
  Serial.println("Serial started");
  radio.begin();
  Serial.println("Radio started");
  radio.openReadingPipe(1,pipe);
  Serial.println("Pipe opened");
}

void loop()
{
  receiveRadio();  
}

void receiveRadio()
{
    if (radio.available)
    {
      
    }
    else
    {
      Serial.println("No radio signal");
    }
}
