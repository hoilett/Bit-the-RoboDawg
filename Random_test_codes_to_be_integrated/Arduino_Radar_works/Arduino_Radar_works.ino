#include <NewPing.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Wire.h>

//edited: Orlando S. Hoilett
//changed trigger and echo pin for use with my Arduino Pro Mini
//and changed max distance to 300
#define TRIGGER_PIN 12 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 11 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define SERVO_PWM_PIN 9
 
// means -angle .. angle
#define ANGLE_BOUNDS 150
#define ANGLE_STEP 10
 
int angle = 0;
 
// direction of servo movement
// -1 = back, 1 = forward
int dir = 1;

String valstring ="";
 
Servo myservo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
 
void setup() {
  Serial.begin(9600);
  myservo.attach(SERVO_PWM_PIN);
}
 
void loop() {
  delay(70);
  // we must renormalize to positive values, because angle is from -ANGLE_BOUNDS .. ANGLE_BOUNDS
  // and servo value must be positive
  myservo.write(angle + ANGLE_BOUNDS);
  
  valstring = String(angle + ANGLE_BOUNDS);    
  if (valstring.length() < 2)   {
      valstring = "00" + valstring;
      }
  else if (valstring.length() < 3)  {
      valstring = "0" + valstring;
      }
  
  // read distance from sensor and send to serial
  getDistanceAndSend2Serial(angle);
  // calculate angle
  if (angle >= ANGLE_BOUNDS || angle <= -ANGLE_BOUNDS) {
  dir = -dir;
  }
  angle += (dir * ANGLE_STEP);
}
 
 
//HELPER METHODS
int getDistanceAndSend2Serial(int angle) {
 
  int cm = sonar.ping_cm();
  Serial.print(angle, DEC);
  Serial.print(",");
  Serial.println(cm, DEC);
  
  valstring = String(cm);    
  if (valstring.length() < 2)   {
      valstring = "00" + valstring;
      }
  else if (valstring.length() < 3)  {
      valstring = "0" + valstring;
      }
}
