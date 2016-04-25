#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

int l1_motor = 10;
int l2_motor = 9;
int r1_motor = 6;
int r2_motor = 5; 


int messageLength = 12;
int msg[1];
RF24 radio(7,8);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";
char theChar = 0;

int xBuffer[];
int yBuffer[];

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
  theChar = msg[0];
  
  if(msg[0] == '*')
  {
    radio.read(msg, 1);
    theChar = msg[0];
    while(msg[0] != '&')
    {
      if (theChar != NULL){
      theMessage.concat(theChar);
      }
      radio.read(msg, 1);
      theChar = msg[0];
    }
 
    //Serial.print("this message: ");
    //Serial.println(theMessage);

    //theMessage = "10,2,5";
    int delimiter = theMessage.indexOf(',');
    //Serial.print("delimiter: ");
    //Serial.println(delimiter);
    
    
    //delimiter = theMessage.indexOf(',');
    int delimiter2 = theMessage.indexOf(',', (delimiter+1));
    int length = theMessage.length();
    char charBuf[length];
    
    theMessage.substring(0,delimiter).toCharArray(charBuf,length);
    int buttonVal = atof(charBuf);
    
    theMessage.substring(delimiter+1, delimiter2).toCharArray(charBuf,length);
    int xDir = atof(charBuf);

    theMessage.substring(delimiter2+1, theMessage.length()).toCharArray(charBuf,length);
    int yDir = atof(charBuf);

    if (xDir <= 1023 && yDir <= 1023)
    {
      xDir = map(xDir, 0, 1023, -255, 255);
      yDir = map(yDir, 0, 1023, -255, 255);
    }



    if(xDir > 10)
    {
      analogWrite(l1_motor, xDir);
      analogWrite(l2_motor, 0);
      
      analogWrite(r1_motor, xDir);
      analogWrite(r2_motor, 0);
    }
    else if(xDir < -10)
    {
      analogWrite(l2_motor, abs(xDir));
      analogWrite(l1_motor, abs(0));
      
      analogWrite(r2_motor, abs(xDir));
      analogWrite(r1_motor, abs(0));
    }

    else 
    {
      analogWrite(l2_motor, 0);
      analogWrite(l1_motor, abs(0));
      
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, abs(0));      
    }

    //Serial.println(theMessage);
    Serial.print(buttonVal); Serial.print('\t');
    Serial.print(xDir); Serial.print('\t');
    Serial.println(yDir);


    theMessage = "";

    
  }
  delay(10);
}
