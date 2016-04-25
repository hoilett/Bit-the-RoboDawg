#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <AFMotor.h> //Motor library
AF_DCMotor fright(1); //front right wheel is controlled by M1 on SainSmart motor driver shield
AF_DCMotor fleft(2); //front left wheel is controlled by M2 on SainSmart motor driver shield
AF_DCMotor bright(3); //back right wheel is controlled by M3 on SainSmart motor driver shield
AF_DCMotor bleft(4); //back left wheel is controlled by M4 on SainSmart motor driver shield


int messageLength = 12;
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";

void setup(void)
{
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop(void)
{
  while(!radio.available());

  radio.read(msg, 1);
  char theChar = msg[0];
  
  if(msg[0] == '*')
  {
    radio.read(msg, 1);
    char theChar = msg[0];
    while(msg[0] != '&')
    {
      if(theChar != NULL)
      {
        theMessage.concat(theChar);
      }
      radio.read(msg, 1);
      char theChar = msg[0];
    }
    
    int delimiter = theMessage.indexOf(',');
    int delimiter2 = theMessage.indexOf(',', (delimiter+1));
    int length = theMessage.length();
    char charBuf[length];
    
    theMessage.substring(0,delimiter).toCharArray(charBuf,length);
    int buttonVal = atof(charBuf);
    
    theMessage.substring(delimiter+1, delimiter2).toCharArray(charBuf,length);
    int xDir = atof(charBuf);

    theMessage.substring(delimiter2+1, theMessage.length()).toCharArray(charBuf,length);
    int yDir = atof(charBuf);

    xDir = map(xDir, 0, 1023, 0, 255);
    yDir = map(yDir, 0, 1023, 0, 255);
    
    fright.setSpeed(xDir);
    fleft.setSpeed(xDir);
    bright.setSpeed(xDir);
    bleft.setSpeed(xDir);

    if(xDir > 600)
    {
      fright.run(FORWARD);
      fleft.run(FORWARD);
      bright.run(FORWARD);
      bleft.run(FORWARD);
    }
    else if(xDir < 450)
    {
      fright.run(BACKWARD);
      fleft.run(BACKWARD);
      bright.run(BACKWARD);
      bleft.run(BACKWARD);
    }

    Serial.println(theMessage);
    Serial.println(buttonVal);
    Serial.println(xDir);
    Serial.println(yDir);

    theMessage = "";
  }
}

