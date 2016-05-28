/*  FILENAME: BitController
 *  AUTHORS:  Orlando S. Hoilett
 *            Shadman Jubaer
 *            Curtis Slaubaugh
 *            Axel Masquelin
 *            Gregory Bergland
 *  VERSION:  V.0.1.0
 *  EMAIL:    orlandohoilett@gmail.com
 *  WEBSITE:  https://github.com/hoilett/Bit-the-RoboDawg.git
 *            https://hackaday.io/project/11905-bit-the-robodawg
 *  
 *  
 *  Description
 *  Code determines basic "follow-me" action for Bit the RoboDawg.
 *  
 *  
 *  Disclaimer
 *  This method is in the public domain. Please feel free to modify, use,
 *  distribute as you wish. It is also provided without warranty.
 *  
 *  If you find any bugs or have any questions, please feel free to contact
 *  orlandohoilett@gmail.com
 *  
 */


//include Bluetooth connectivity for debugging
#include <SoftwareSerial.h>
#define BT_SERIAL_TX_DIO 2
#define BT_SERIAL_RX_DIO 3
SoftwareSerial BT(BT_SERIAL_TX_DIO, BT_SERIAL_RX_DIO);

//BitReceptor analog inputs for each cardinal point
const uint8_t north = A0;
const uint8_t west = A1;
const uint8_t east = A2;
const uint8_t south = A3;

//BitReceptor intensity values for each cardinal point
double nVal = 0;
double wVal = 0;
double eVal = 0;
double sVal = 0;

//BitReceptor array indexes for each cardinal points
int n = 0;
int w = 1;
int e = 2;
int s = 3;

//size of array holding intensity values for each cardinal point on the BitReceptor
const int arr_size = 4;

//initializing array
double sensorVals[arr_size] = {0};

//baseline intensity values for each cardinal point on the BitRecepto
double n_base = 0;
double w_base = 0;
double e_base = 0;
double s_base = 0;

//Motor control output pins for the BitDriver
//Write HIGH to r1 or l1 and LOW to r2 or l2 to move motors forward
//Write HIGH to r2 or l2 and LOW to r1 or l1 to move motors backward
//Write HIGH to r1 and a fraction of HIGH to l1 and LOW to r2 and l2 to turn left
//Write HIGH to l1 and a fraction of HIGH to r1 and LOW to l2 and r2 to turn righ
//Write HIGH to r1 and LOW to r2 and HIGH to l2 and LOW to l1 to turn left on a dime
//Write HIGH to l1 and LOW to l2 and HIGH to r2 and LOW to r1 to turn right on a dime
//Write LOW to r1, r2, l1, and l2 to stop
int r2_motor = 6;
int r1_motor = 9;
int l1_motor = 11;
int l2_motor = 10; 

//threshold intensity values for each cardinal point
int move_threshold = 55;

void setup()
{
  Serial.begin(115200);
  BT.begin(9600);
  delay(2000);

  //at the start of the program, the BitController takes sample intensity values
  //for each cardinal point and averages the values to set baseline intensities
  int max_n = -1023;
  int max_w = -1023;
  int max_e = -1023;
  long timer1 = millis();
  int temp_north = 0;
  int temp_west = 0;
  int temp_east = 0;
    
  for (int i = 0; i < 10; i++)
  {
    temp_north += analogRead(north);
    temp_west += analogRead(west);
    temp_east += analogRead(east);
    
    delay(10);
  }

  //-20 is used as a bit of wiggle room to set the baseline values around 20
  //preventing the intensity values from going below 0
  n_base = temp_north/10 - 20;
  w_base = temp_west/10 - 20;
  e_base = temp_east/10 - 20;


  //printing baseline values to Bluetooth and Serial Monitor
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


//void readSensors()
//This method reads the intensities at each cardinal point on the BitReceptor
//and takes an average of a number of readings. The averaged values are placed
//in the sensor values array.
void readSensors()
{
  int north_temp = 0;
  int west_temp = 0;
  int east_temp = 0;

  //sums up 10 sequential readings of each cardinal point
  for (int i = 0; i < 10; i++)
  {
    north_temp += analogRead(north);
    west_temp += analogRead(west);
    east_temp += analogRead(east);
    delay(20);
  }

  //averages the values of 10 readings and subtracts the baseline value to
  //correct for zero
  nVal = north_temp/10.0 - n_base;
  wVal = west_temp/10.0 - w_base;
  eVal = east_temp/10.0 - e_base;

  sensorVals[n] = nVal;
  sensorVals[w] = wVal;
  sensorVals[e] = eVal;
  //sensorVals[s] = sVal;  
}


//void printSensors()
//Prints sensor values to connected Bluetooth device and to the Serial Monitor
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
  
}


//void loop()
//Reads sensor values, prints sensor Values, adjusts direction of Bit based
//on sensor values of each cardinal point
void loop()
{
  readSensors();
  printSensors();
  determineDirection();
  //drive(sensorVals[n], sensorVals[s], sensorVals[e], sensorVals[w]);
  
  BT.println();
  BT.println();
  Serial.println();
  Serial.println();

  delay(10);
}



//void move()
//Determines movement direction for Bit based on intensity values of each
//sensor. this method has been replaced by the determineDirection() method
// as of Version 0.1.0 and was only used as a test with the north and east
//cardinal points
void move()
{
  printSensors();

  //Check if movement threshold has been reached by any of the sensors.
  if(sensorVals[n] > move_threshold || sensorVals[w] > move_threshold || sensorVals[e] > move_threshold)
  {
    //If north intensity is higher than east intensity, the Bit should move
    //due north.
    if ((sensorVals[n]*.6) > sensorVals[e])
    {
      BT.print("go north");
      Serial.print("go north");
      analogWrite(l1_motor, 255);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 255);
      analogWrite(r2_motor, 0);    
    }
    //If east intensity is higher than north intensity, the Bit should move
    //due east.
    else if ((sensorVals[e]*.6) > sensorVals[n])
    {
      BT.print("go east");
      Serial.print("go east");
      analogWrite(l1_motor, 255);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, 255);
    }
    //If the north and east intensity ratios are comparable, then Bit should
    //move due north-east. If the ratio of north intensity to east intensity is
    //1, then the intensities are equal. If the ratio is within 0.15 of 1, so
    //either 0.85 and 1.15, then the intensities are comparable and Bit moves
    //due north-east.
    else if (abs(((double)sensorVals[n]/(double)sensorVals[e]) - 1)  < 0.15)
    {
      BT.print("go north-east");
      Serial.print("go north-east");
      analogWrite(l1_motor, 255);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 55);
      analogWrite(r2_motor, 0);
    }
    //If no direction can be determined, move along previously determined path
    else
    {
      //some error
      //continue along previous path 
      BT.print("don't know");
      Serial.print("don't know");
    }
  }
  //if movement threshold has not been reached, Bit will stop
  else
  {
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


//void determineDirection()
//Determines movement direction for Bit based on intensity values of each sensor.
void determineDirection()
{
  if(sensorVals[n] > move_threshold || sensorVals[w] > move_threshold || sensorVals[e] > move_threshold)
  {
    //If north intensity is greater than both west and east intensity, the
    //BitBeacon must be coming from north. 
    if ((sensorVals[n]*.7) > sensorVals[w] || (sensorVals[n]*.7) > sensorVals[e])
    {
      //if the west and east intensities are very close, this means that the
      //BitBeacon is coming directly from north and Bit should head north
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
      //If west intensity is greater than east intensity, then this means
      //that the BitBeacon signal is coming from north-west
      //the right motors are driven faster than the left motors so that Bit
      //turns left (west)
      else if (sensorVals[w]*1.2 >= sensorVals[e]*1.2)
      {
        analogWrite(l1_motor, 50);
        analogWrite(l2_motor, 0);
        analogWrite(r1_motor, 230);
        analogWrite(r2_motor, 0);
        //lcd.println("head north-west");
        Serial.println("head north-west");
        BT.println("head north-west");
      }
      //If east intensity is greater than east intensity, then this means
      //that the BitBeacon signal is coming from north-east
      //the left motors are driven faster than the right motors so that Bit
      //turns right (east)
      else if (sensorVals[e]*1.2 >= sensorVals[w]*1.2)
      {
        analogWrite(l1_motor, 230);
        analogWrite(l2_motor, 0);
        analogWrite(r1_motor, 50);
        analogWrite(r2_motor, 0);
        //lcd.println("head north-east");
        Serial.println("head north-east");
        BT.println("head north-east");
      }
      //if neither conditions above are met, then Bit contiues along the
      //previous known direction
      else
      {
        //lcd.println("no direction or continue moving along last direction");
        Serial.println("no direction or continue moving along last direction");
        BT.println("no direction or continue moving along last direction");
        //some error
      }
    }
    
    //if the east intensity value is greater than the north and west intensity
    //value, then Bit should move due east. Bit heads east (right) by driving
    //the left motors forward and right motors backwards so Bit turns right in
    //one spot.
    //As of Version 0.1.0. the delineation between east and north-east is a fuzzy
    else if ((sensorVals[e]*.65) > sensorVals[n] && sensorVals[e]*.9 > sensorVals[w])
    {
      analogWrite(l1_motor, 200);
      analogWrite(l2_motor, 0);
      analogWrite(r1_motor, 0);
      analogWrite(r2_motor, 200);
      Serial.println("head east");
      BT.println("head east");
    }
  
    //if the west intensity value is greater than the north and west intensity
    //value, then Bit should move due west. Bit heads west (left) by driving
    //the right motors forward and left motors backwards so Bit turns left in
    //one spot.
    //As of Version 0.1.0. the delineation between west and north-west is a fuzzy
    else if ((sensorVals[w]*.65) > sensorVals[n] && sensorVals[w]*.9 > sensorVals[e])
    {
        analogWrite(l1_motor, 0);
        analogWrite(l2_motor, 200);
        analogWrite(r1_motor, 200);
        analogWrite(r2_motor, 0);
       Serial.println("head west");
       BT.println("head west");
    }

    //If no direction can be determined, Bit continues along the previously
    //determined direction.
    else
    {
      Serial.println("got no where");
      BT.println("got no where");
    }
  }
  
  //If a movement threshold has not been crossed, Bit stops moving.
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


//void drive(double nx, double sx, double ex, double wx)
//@param      double nx       north intensity value
//@param      double sx       south intensity value
//@param      double ex       east intensity value
//@param      double wx       west intensity value
//
//Determines the motor speeds by directly writing normalized cardinal point
//intensity valules.
//
//This method is unstable and is currently replaced by the determineDirection()
//method as of Version 0.1.0
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

  //determining control of the left motors 
  //If north intensity is the highest, val_ns will be much greater than val_ew.
  //Bit will then head due north. Else, south intensity is higher and val_ns - val_ew
  //will be less than 0. Bit will head due south.
  if (val_ns - val_ew > 0) //Left motors
  {
    analogWrite(l1_motor, val_ns - val_ew); //(l1 high & l2 low, motors go forward)
    analogWrite(l2_motor, 0);
  }
  else
  {
    analogWrite(l1_motor, 0);
    analogWrite(l2_motor, abs(val_ns - val_ew));
  }

  //determining control of the right motors
  //If north intensity is the highest, val_ns will be much greater than val_ew.
  //Bit will then head due north. Else, south intensity is higher and val_ns - val_ew
  //will be less than 0. Bit will head due south.
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

}

