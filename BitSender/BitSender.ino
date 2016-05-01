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

int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t button = A0;
uint8_t xdir = A1;
uint8_t ydir = A2;

void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(pipe);
}

void loop()
{
  String theMessage = "*" + String(analogRead(button)) + "," + String(analogRead(xdir)) + "," + String(analogRead(ydir));
  //String theMessage = String(analogRead(button)) + "," + String(analogRead(xdir)) + "," + String(analogRead(ydir));
  int messageSize = theMessage.length();

  for (int i = 0; i < messageSize; i++)
  {
    int charToSend[1];
    charToSend[0] = theMessage.charAt(i);
    radio.write(charToSend, 1);
    Serial.println(theMessage);
  }

  //send the 'terminate string' value
  msg[0] = '&';
  //msg[0] = 2;
  radio.write(msg,1);

  delay(5);
}
