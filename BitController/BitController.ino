//LCD
//#include <Wire.h>
//#include <LCD.h>
//#include <LiquidCrystal_I2C.h>
//
//#define I2C_ADDR  0x3F  // Define I2C Address where the PCF8574A is
//#define BACKLIGHT_PIN   3
//#define En_pin  2
//#define Rw_pin  1
//#define Rs_pin  0
//#define D4_pin  4
//#define D5_pin  5
//#define D6_pin  6
//#define D7_pin  7
//
//LiquidCrystal_I2C     lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


#include <SoftwareSerial.h>
#define BT_SERIAL_TX_DIO 2
#define BT_SERIAL_RX_DIO 3

SoftwareSerial BT(BT_SERIAL_TX_DIO, BT_SERIAL_RX_DIO);


const uint8_t north = A0;
const uint8_t west = A1;
const uint8_t east = A2;
const uint8_t south = A3;

double nVal = 0;
double wVal = 0;
double eVal = 0;
double sVal = 0;

int n = 0;
int w = 1;
int e = 2;
int s = 3;

const int arr_size = 4;

double sensorVals[arr_size] = {0};

double n_base = 0;
double w_base = 0;
double e_base = 0;
double s_base = 0;


//motor control
int r2_motor = 6;
int r1_motor = 9;
int l1_motor = 11;
int l2_motor = 10; 

void setup()
{
  Serial.begin(115200);
  BT.begin(9600);

//  lcd.begin (20,4);
//  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
//  lcd.setBacklight(HIGH);
//  lcd.home ();

  delay(2000);

  int max_n = -1023;
  int max_w = -1023;
  int max_e = -1023;
  long timer1 = millis();

  while((timer1 - millis()) < 500)
  {
    int temp_north = analogRead(north);
    int temp_west = analogRead(west);
    int temp_east = analogRead(east);
    
    if(temp_north > max_n) max_n = temp_north;
    if(temp_west > max_w) max_w = temp_west;
    if(temp_east > max_e) max_e = temp_east;
    delay(10);
  }

  n_base = max_n - 20;
  w_base = max_w - 20;
  e_base = max_e - 20;

//  n_base = analogRead(north) - 50;
//  w_base = analogRead(west) - 50;
//  e_base = analogRead(east) - 50;
  //s_base = analogRead(south) - 50;

//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print(n_base);
//  lcd.setCursor(0,1);
//  lcd.print(w_base);
//  lcd.setCursor(0,2);
//  lcd.print(e_base);
//  delay(3000);
  //lcd.println(s_base);
  
  BT.print("north: ");
  BT.print(n_base);
  BT.print("\t");
  BT.print("west: ");
  BT.print(w_base);
  BT.print("\t");
  BT.print("east: ");
  BT.print(e_base);
  BT.println();
  BT.println();

  Serial.print("north: ");
  Serial.print(n_base);
  Serial.print("\t");
  Serial.print("west: ");
  Serial.print(w_base);
  Serial.print("\t");
  Serial.print("east: ");
  Serial.print(e_base);
  Serial.println();
  Serial.println();
}

void readSensors()
{
  int north_temp = 0;
  int west_temp = 0;
  int east_temp = 0;
  //int north_temp = 0;
  for (int i = 0; i < 10; i++)
  {
    north_temp += analogRead(north);
    west_temp += analogRead(west);
    east_temp += analogRead(east);
    delay(50);
  }

  nVal = north_temp/10.0 - n_base;
  wVal = west_temp/10.0 - w_base;
  eVal = east_temp/10.0 - e_base;

//  int north_intensity = analogRead(north);
//  int west_intensity = analogRead(west);
//  int east_intensity = analogRead(east);
  //int south_intensity = analogRead(south);
  
//  nVal = north_intensity - n_base;
//  wVal = west_intensity - w_base;
//  eVal = east_intensity - e_base;
  //sVal = south_intensity - s_base;

  //sensorVals[0] = {nVal, eVal, sVal, wVal};
  sensorVals[n] = nVal;
  sensorVals[w] = wVal;
  sensorVals[e] = eVal;
  //sensorVals[s] = sVal;  
}


void printSensors()
{
  BT.print("north: ");
  BT.print(nVal);
  BT.print("\t");
  BT.print("west: ");
  BT.print(wVal);
  BT.print("\t");
  BT.print("east: ");
  BT.println(eVal);
//  BT.println();
//  BT.println();

  Serial.print("north: ");
  Serial.print(nVal);
  Serial.print("\t");
  Serial.print("west: ");
  Serial.print(wVal);
  Serial.print("\t");
  Serial.print("east: ");
  Serial.println(eVal);
//  Serial.println();
//  Serial.println();
  
//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print("north ");
//  lcd.print(nVal);
//  //lcd.print(", ");
//
//  lcd.setCursor(0,1);
//  lcd.print("west: ");
//  lcd.print(wVal);
//  //lcd.print(", ");
//
//  lcd.setCursor(0,2);
//  lcd.print("east ");
//  lcd.print(eVal);
  //lcd.print(", ");
  
//  lcd.print("south: ");
//  lcd.print(sVal);
//  lcd.print(", ");
  

}


void loop()
{
  readSensors();
  printSensors();
  //sensorVals[s] = 300;
  determineDirection();
  //drive(sensorVals[n], sensorVals[s], sensorVals[e], sensorVals[w]);
  
  BT.println();
  BT.println();
  Serial.println();
  Serial.println();
  //BT.println("hello world");
  //dmove();
  //determineDirection();
  //drive(sensorVals[n], sensorVals[s], sensorVals[e], sensorVals[w]);
  //lcd.println();
  //delay(1500);
}


void move()
{
  printSensors();
  //lcd.setCursor(0,3);
  if(sensorVals[n] > 130 || sensorVals[w] > 130 || sensorVals[e] > 130)
  {
  if ((sensorVals[n]*.6) > sensorVals[e])
  {
    //go north
    //lcd.print("go north");
    BT.print("go north");
    Serial.print("go north");
    analogWrite(l1_motor, 255);
    analogWrite(l2_motor, 0);
    analogWrite(r1_motor, 255);
    analogWrite(r2_motor, 0);
    
  }
  else if ((sensorVals[e]*.6) > sensorVals[n])
  {
    //go east
    //lcd.print("go east");
    BT.print("go east");
    Serial.print("go east");
    analogWrite(l1_motor, 255);
    analogWrite(l2_motor, 0);
    analogWrite(r1_motor, 0);
    analogWrite(r2_motor, 255);
  }
  else if (abs(((double)sensorVals[n]/(double)sensorVals[e]) - 1)  < 0.15)
  {
    //go north east
    //lcd.print("go north-east");
    BT.print("go north-east");
    Serial.print("go north-east");
    analogWrite(l1_motor, 255);
    analogWrite(l2_motor, 0);
    analogWrite(r1_motor, 55);
    analogWrite(r2_motor, 0);
  }
  else
  {
    //some error
    //continue along previous path 
    //lcd.print("don't know");
    BT.print("don't know");
    Serial.print("don't know");
  }
  }
  else
  {
    //lcd.print("no signal");
    BT.print("no signal");
    Serial.print("no signal");
    analogWrite(l1_motor, 0);
    analogWrite(l2_motor, 0);
    analogWrite(r1_motor, 0);
    analogWrite(r2_motor, 0);
  }

//  BT.println();
//  BT.println();
//  Serial.println();
//  Serial.println();
}



void determineDirection()
{
  //check to see which direction has highest value
  //check to see which direction is second
  //check to see which direction is third
  if(sensorVals[n] > 80 || sensorVals[w] > 80 || sensorVals[e] > 80)
  {
  if ((sensorVals[n]*.7) > sensorVals[w] || (sensorVals[n]*.7) > sensorVals[e])
  {
    if (abs(sensorVals[w]-sensorVals[e]) < 50)
    {
      analogWrite(l1_motor, 200);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 200);
      analogWrite(r2_motor, 0);
      //lcd.println("head north");
      Serial.println("head north");
      BT.println("head north");
    }
    else if (sensorVals[w]*1.2 >= sensorVals[e]*1.2)
    {
      analogWrite(l1_motor, 50);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 200);
      analogWrite(r2_motor, 0);
      //lcd.println("head north-west");
      Serial.println("head north-west");
      BT.println("head north-west");
    }
    else if (sensorVals[e]*1.2 >= sensorVals[w]*1.2)
    {
      analogWrite(l1_motor, 200);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 50);
      analogWrite(r2_motor, 0);
      //lcd.println("head north-east");
      Serial.println("head north-east");
      BT.println("head north-east");
    }
    else
    {
      //lcd.println("no direction or continue moving along last direction");
      Serial.println("no direction or continue moving along last direction");
      BT.println("no direction or continue moving along last direction");
      //some error
    }
  }
  //else if ((sensorVals[e]*.6) > sensorVals[n] && (sensorVals[e]*.6) > sensorVals[s])
  else if ((sensorVals[e]*.65) > sensorVals[n] && sensorVals[e]*.9 > sensorVals[w])
  //else if ((sensorVals[e]*.65) > sensorVals[n] && sensorVals[e] > 100)
  {
      analogWrite(l1_motor, 200);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, 200);
    Serial.println("head east");
    BT.println("head east");
//    if (abs(sensorVals[n]-sensorVals[s]) < 50)
//    {
//      //lcd.println("head east");
//      Serial.println("head east");
//      BT.println("head east");
//    }
//    else if (sensorVals[n]*1.2 >= sensorVals[s]*1.2)
//    {
//      //lcd.println("head east-north-east");
//      Serial.println("head east-north-east");
//      BT.println("head east-north-east");
//    }
//    else if (sensorVals[s]*1.2 >= sensorVals[n]*1.2)
//    {
//      //lcd.println("head east-south-east");
//      Serial.println("head east-south-east");
//      BT.println("head east-south-east");
//    }
//    else
//    {
//      //lcd.println("no direction or continue moving along last direction");
//      Serial.println("no direction or continue moving along last direction");
//      BT.println("no direction or continue moving along last direction");
//      //some error
//    }
  }
  //else if ((sensorVals[w]*.6) > sensorVals[n] && (sensorVals[w]*.6) > sensorVals[s])
  else if ((sensorVals[w]*.65) > sensorVals[n] && sensorVals[w]*.9 > sensorVals[e])
  //else if ((sensorVals[w]*.65) > sensorVals[n]  && sensorVals[w > 100)
  {
      analogWrite(l1_motor, 0);
      analogWrite(l2_motor, 200);
      analogWrite(r1_motor, 200);
      analogWrite(r2_motor, 0);
     Serial.println("head west");
     BT.println("head west");
//    if (abs(sensorVals[n]-sensorVals[s]) < 50)
//    {
//     // lcd.println("head west");
//     Serial.println("head west");
//     BT.println("head west");
//    }
//    else if (sensorVals[n]*1.2 >= sensorVals[s]*1.2)
//    {
//      //lcd.println("head west-north-west");
//      Serial.println("head west-north-west");
//      BT.println("head west-north-west");
//    }
//    else if (sensorVals[s]*1.2 >= sensorVals[n]*1.2)
//    {
//      //lcd.println("head west-south-west");
//      Serial.println("head west-south-west");
//      BT.println("head west-south-west");
//    }
//    else
//    {
//      //lcd.println("no direction or continue moving along last direction");
//      Serial.println("no direction or continue moving along last direction");
//      BT.println("no direction or continue moving along last direction");
//      //some error
//    }
  }
//  else if ((sensorVals[s]*.6) > sensorVals[w] && (sensorVals[s]*.6) > sensorVals[e])
//  {
//    if (abs(sensorVals[w]-sensorVals[e]) < 50)
//    {
//      //lcd.println("head south");
//    }
//    else if (sensorVals[w]*1.2 >= sensorVals[e]*1.2)
//    {
//      //lcd.println("head south-west");
//    }
//    else if (sensorVals[e]*1.2 >= sensorVals[w]*1.2)
//    {
//      //lcd.println("head south-east");
//    }
//    else
//    {
//      //lcd.println("no direction or continue moving along last direction");
//      //some error
//    }
//  }
  else
  {
    //lcd.println("got no where");
    Serial.println("got no where");
    BT.println("got no where");
  }
  }
  else
  {
      analogWrite(l1_motor, 0);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, 0);
     Serial.println("no signal");
     BT.println("no signal");
    
  }
}


void drive(double nx, double sx, double ex, double wx)
{
  int val_ns = 0;
  int val_ew = 0;
  int val1 = 0;
  int val2 = 0;
  int val3 = 0;
  
  val_ns = map(log(nx/sx), -log(30), log(30), -127, 127); //S at -127, N at 127
  val_ew = map(log(ex/wx), -log(30), log(30), -127, 127); //W at -127, E at 127
  //The summed/subtracted range of val_ns with val_ew is +/-254

  Serial.print("val_ns: ");
  Serial.print(val_ns);
  Serial.print("\t");
  Serial.print("val_ew: ");
  Serial.println(val_ew);

  BT.print("val_ns: ");
  BT.print(val_ns);
  BT.print("\t");
  BT.print("val_ew: ");
  BT.println(val_ew);

  if (val_ns - val_ew > 0) //Left motors
  {
//    for (int i = 0; i < val_ns - val_ew; i++)
//    {
//      analogWrite(l1_motor, i);
//      delay(1);
//     
//    }
    analogWrite(l1_motor, val_ns - val_ew); //(l1 high & l2 low, motors go forward)
    analogWrite(l2_motor, 0);
  }
  else
  {
    analogWrite(l1_motor, 0);
    analogWrite(l2_motor, abs(val_ns - val_ew));
  }

  if (val_ns + val_ew > 0) //Right motors
  {
    analogWrite(r1_motor, val_ns + val_ew);
    analogWrite(r2_motor, 0);
  }
  else
  {
    analogWrite(r1_motor, 0);
    analogWrite(r2_motor, abs(val_ns + val_ew));
  }

  //val1 = map(nx/ex, , , -255, 255)
  //val2 = map(nx/wx, , , -255, 255)

  //north
  //if (nx <= 50) nx = 0;
  //sx = map(nx, 0, 600, 0, 255);
  //analogWrite(l1_motor, nx);
  //analogWrite(l2_motor, 0);
  //analogWrite(r1_motor, nx);
  //analogWrite(r2_motor, 0);

}

