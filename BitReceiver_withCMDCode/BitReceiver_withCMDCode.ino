#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>


int messageLength = 12;
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";
char theChar = 0;

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
    double buttonVal = atof(charBuf);
    
    theMessage.substring(delimiter+1, delimiter2).toCharArray(charBuf,length);
    double xDir = atof(charBuf);

    theMessage.substring(delimiter2+1, theMessage.length()).toCharArray(charBuf,length);
    double yDir = atof(charBuf);

    Serial.println(theMessage);
    Serial.println(buttonVal);
    Serial.println(xDir);
    Serial.println(yDir);

    theMessage = "";
  }
  delay(10);
}
