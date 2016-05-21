//LCD
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR  0x3F  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN   3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C     lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


const uint8_t north = A0;
const uint8_t west = A1;
const uint8_t east = A2;
const uint8_t south = A3;

int nVal = 0;
int wVal = 0;
int eVal = 0;
int sVal = 0;

int n = 0;
int w = 1;
int e = 2;
int s = 3;

const int arr_size = 4;

int sensorVals[arr_size] = {0};

int n_base = 0;
int w_base = 0;
int e_base = 0;
int s_base = 0;


//motor control
int l1_motor = 10;
int l2_motor = 9;
int r1_motor = 6;
int r2_motor = 5; 

void setup()
{
  Serial.begin(115200);

  lcd.begin (20,4);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();

  delay(1000);

  n_base = analogRead(north) - 50;
  w_base = analogRead(west) - 50;
  e_base = analogRead(east) - 50;
  //s_base = analogRead(south) - 50;
  
  lcd.println(n_base);
  lcd.println(w_base);
  lcd.println(e_base);
  //lcd.println(s_base);
  
}


void readSensors()
{
  int north_intensity = analogRead(north);
  int west_intensity = analogRead(west);
  int east_intensity = analogRead(east);
  //int south_intensity = analogRead(south);
  
  
  nVal = north_intensity - n_base;
  wVal = west_intensity - w_base;
  eVal = east_intensity - e_base;
  //sVal = south_intensity - s_base;
  

  //sensorVals[0] = {nVal, eVal, sVal, wVal};
  sensorVals[n] = nVal;
  sensorVals[w] = wVal;
  sensorVals[e] = eVal;
  //sensorVals[s] = sVal;
  
}


void printSensors()
{
  lcd.print("north: ");
  lcd.print(nVal);
  lcd.print(", ");

  lcd.print("west: ");
  lcd.print(wVal);
  lcd.print(", ");
  
  lcd.print("east: ");
  lcd.println(eVal);
  //lcd.print(", ");
  
//  lcd.print("south: ");
//  lcd.print(sVal);
//  lcd.print(", ");
  

}


void loop()
{
  readSensors();
  printSensors();
  //determineDirection();
  //drive(sensorVals[n], sensorVals[s], sensorVals[e], sensorVals[w]);
  //lcd.println();
  delay(500);
}



void determineDirection()
{
  //check to see which direction has highest value
  //check to see which direction is second
  //check to see which direction is third

  if ((sensorVals[n]*.6) > sensorVals[w] && (sensorVals[n]*.6) > sensorVals[e])
  {
    if (abs(sensorVals[w]-sensorVals[e]) < 50)
    {
      lcd.println("head north");
    }
    else if (sensorVals[w]*1.2 >= sensorVals[e]*1.2)
    {
      lcd.println("head north-west");
    }
    else if (sensorVals[e]*1.2 >= sensorVals[w]*1.2)
    {
      lcd.println("head north-east");
    }
    else
    {
      lcd.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else if ((sensorVals[e]*.6) > sensorVals[n] && (sensorVals[e]*.6) > sensorVals[s])
  {
    if (abs(sensorVals[n]-sensorVals[s]) < 50)
    {
      lcd.println("head east");
    }
    else if (sensorVals[n]*1.2 >= sensorVals[s]*1.2)
    {
      lcd.println("head east-north-east");
    }
    else if (sensorVals[s]*1.2 >= sensorVals[n]*1.2)
    {
      lcd.println("head east-south-east");
    }
    else
    {
      lcd.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else if ((sensorVals[w]*.6) > sensorVals[n] && (sensorVals[w]*.6) > sensorVals[s])
  {
    if (abs(sensorVals[n]-sensorVals[s]) < 50)
    {
      lcd.println("head west");
    }
    else if (sensorVals[n]*1.2 >= sensorVals[s]*1.2)
    {
      lcd.println("head west-north-west");
    }
    else if (sensorVals[s]*1.2 >= sensorVals[n]*1.2)
    {
      lcd.println("head west-south-west");
    }
    else
    {
      lcd.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else if ((sensorVals[s]*.6) > sensorVals[w] && (sensorVals[s]*.6) > sensorVals[e])
  {
    if (abs(sensorVals[w]-sensorVals[e]) < 50)
    {
      lcd.println("head south");
    }
    else if (sensorVals[w]*1.2 >= sensorVals[e]*1.2)
    {
      lcd.println("head south-west");
    }
    else if (sensorVals[e]*1.2 >= sensorVals[w]*1.2)
    {
      lcd.println("head south-east");
    }
    else
    {
      lcd.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else
  {
    lcd.println("got no where");
  }
}


void drive(int nx, int sx, int ex, int wx)
{
  //north
  if (nx <= 50) nx = 0;
  nx = map(nx, 0, 600, 0, 255);
  analogWrite(l1_motor, nx);
  analogWrite(l2_motor, 0);
  analogWrite(r1_motor, nx);
  analogWrite(r2_motor, 0);
}

