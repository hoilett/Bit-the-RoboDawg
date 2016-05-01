/*
 * Shadman Jubaer and Orlando Hoilett
 * 
 * Project Title: Bit the RoboCar
 * 
 * Description
 * We created a remote controlled car using two nRF24L01 RF
 * transceiver modules. An Arduino reads the directeion values
 * from a joystick and sends the instructions to another Arduino
 * to control a robot chassis.
 * 
 */


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

int xBuffer[5];
int yBuffer[5];
int bufferVal = 0;
int xDir;
int yDir;
int buttonVal;


int zeroLine = 50;

void setup(void)
{
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
      Serial.println("STOP");
      analogWrite(l2_motor, 0);
      analogWrite(l1_motor, 0);
      
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, 0);
}

void loop(void)
{
  while(!radio.available());
  radio.read(msg, 1);
  theChar = msg[0];
  
  if(msg[0] == '*'){
    radio.read(msg, 1);
    theChar = msg[0];
    
    while(msg[0] != '&'){
      
      if (theChar != NULL){
      theMessage.concat(theChar);
      }
      
      radio.read(msg, 1);
      theChar = msg[0];
    }

     if (bufferVal <= 4){
      
      int delimiter = theMessage.indexOf(',');
      int delimiter2 = theMessage.indexOf(',', (delimiter+1));
      int length = theMessage.length();
      char charBuf[length];
      
      theMessage.substring(0,delimiter).toCharArray(charBuf,length);
      buttonVal = atof(charBuf);
      
      theMessage.substring(delimiter+1, delimiter2).toCharArray(charBuf,length);
      xBuffer[bufferVal] = (int)atof(charBuf);
      //Serial.print(xBuffer[bufferVal]); Serial.print('\t');
      
  
      theMessage.substring(delimiter2+1, theMessage.length()).toCharArray(charBuf,length);
      yBuffer[bufferVal] = (int)atof(charBuf);
      //Serial.print(yBuffer[bufferVal]);
      //Serial.print('\t');
      
      //Filter out noise due to high numbers 
      if (xBuffer[bufferVal] <= 1023 && xBuffer[bufferVal] >= 0 && yBuffer[bufferVal] <= 1023 && yBuffer[bufferVal] >= 0)
      {
        bufferVal = bufferVal + 1;
      }

      //Filter out noise due to random shifts
      if(bufferVal >= 2 && xBuffer[bufferVal] == xBuffer[bufferVal-2])
      {
        if (xBuffer[bufferVal-1] != xBuffer[bufferVal] && xBuffer[bufferVal-1] != xBuffer[bufferVal-2])
        {
          xBuffer[bufferVal-1] = xBuffer[bufferVal]; 
        }
        if (yBuffer[bufferVal-1] != yBuffer[bufferVal] && yBuffer[bufferVal-1] != yBuffer[bufferVal-2])
        {
          yBuffer[bufferVal-1] = yBuffer[bufferVal]; 
        } 
      }
     }
     
    else {   
      bufferVal = 0;
      xDir = (xBuffer[0] + xBuffer[1] + xBuffer[2] + xBuffer[3] + xBuffer[4])/5;
      //Serial.print(xDir); Serial.print('\t');
      yDir = (yBuffer[0] + yBuffer[1] + yBuffer[2] + yBuffer[3] + yBuffer[4])/5;
      //Serial.println(yDir);
      //Serial.print('\t');
      
      if (xDir <= 1023 && yDir <= 1023){
        xDir = map(xDir, 0, 1023, -255, 255);
        //Serial.print(xDir); Serial.print('\t');
        yDir = map(yDir, 0, 1023, -255, 255);
        //Serial.println(yDir);
      }
    }
  
    int diff = abs(xDir) - abs(yDir);

    //GO FORWARD
    if(xDir > zeroLine){
      Serial.println("FORWARD");
      analogWrite(l1_motor, xDir);
      analogWrite(l2_motor, 0);
      
      analogWrite(r1_motor, xDir);
      analogWrite(r2_motor, 0);
    }

    //GO BACKWARD
    else if(xDir < -zeroLine){
      Serial.println("BACKWARD");
      analogWrite(l2_motor, abs(xDir));
      analogWrite(l1_motor, abs(0));
      
      analogWrite(r2_motor, abs(xDir));
      analogWrite(r1_motor, abs(0));
    }
    
    //TURN RIGHT
    if(yDir > zeroLine){
      Serial.println("RIGHT");
      analogWrite(l1_motor, yDir);
      analogWrite(l2_motor, 0);
      
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, yDir);
    }

    //TURN LEFT
    else if(yDir < -zeroLine){
      Serial.println("LEFT");
      analogWrite(l2_motor, abs(yDir));
      analogWrite(l1_motor, abs(0));
      
      analogWrite(r2_motor, 0);
      analogWrite(r1_motor, abs(yDir));
    }

    //STOP
    else if (abs(yDir) < abs(zeroLine) && abs(xDir) < abs(zeroLine) ){
      Serial.println("STOP");
      analogWrite(l2_motor, 0);
      analogWrite(l1_motor, 0);
      
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, 0);      
    }
  }  
  //    //Serial.println(theMessage);
  //    Serial.print(buttonVal); Serial.print('\t');
  //    Serial.print(xDir); Serial.print('\t');
  //    Serial.println(yDir);
    theMessage = "";
    //delay(1);
}
