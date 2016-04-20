#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>


//JOYSTICK
const int SW_pin = 3; //Digital pin connected to switch input
const int X_pin = A0; //Analog pin connected to X output
const int Y_pin = A1; //Analog pin connecte to Y output
int x_joy;            //Joystick X Output
int y_joy;            //Joystick Y Output
int sw_joy;           //Joystick SW Output

/*
This sketch sends a string to a corresponding Arduino
with nrf24 attached.  It appends a specific value 
(2 in this case) to the end to signify the end of the
message.
*/

int msg[3];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void)
{
  Serial.begin(9600);           //Begin serial
  radio.begin();                //Begin radio
  radio.openWritingPipe(pipe);  //Open pipe
  pinMode(SW_pin, INPUT);       //Set SW pin as input
  digitalWrite(SW_pin, HIGH);
}

void loop(void)
{
  x_joy = analogRead(X_pin);
  y_joy = analogRead(Y_pin);
  sw_joy = digitalRead(SW_pin);
  msg[0] = sw_joy;
  msg[1] = x_joy;
  msg[2] = y_joy;
  radio.write(msg,3);
//  Serial.print(msg[0]);
//  Serial.print('\t');
//  Serial.print(msg[1]);
//  Serial.print('\t');
//  Serial.println(msg[2]);
/*delay sending for a short period of time.  radio.powerDown()/radio.powerupp
//with a delay in between have worked well for this purpose(just using delay seems to
//interrupt the transmission start). However, this method could still be improved
as I still get the first character 'cut-off' sometimes. I have a 'checksum' function
on the receiver to verify the message was successfully sent.
*/
  radio.powerDown(); 
  delay(100);
  radio.powerUp();
}
