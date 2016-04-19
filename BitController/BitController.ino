const uint8_t north = A0;
const uint8_t east = A1;
const uint8_t south = A2;
const uint8_t west = A3;

int nVal = 0;
int eVal = 0;
int sVal = 0;
int wVal = 0;

int n = 0;
int e = 1;
int s = 2;
int w = 3;
const int arr_size = 4;

int sensorVals[arr_size] = {0};

int n_base = 0;
int e_base = 0;
int s_base = 0;
int w_base = 0;

uint8_t interrupt = 2;

void setup()
{
  //analogReference(EXTERNAL);
  Serial.begin(115200);

  delay(1000);

  n_base = analogRead(north) - 50;
  e_base = analogRead(east) - 50;
  s_base = analogRead(south) - 50;
  w_base = analogRead(west) - 50;

  Serial.println(n_base);
  Serial.println(e_base);
  Serial.println(s_base);
  Serial.println(w_base);
}


void readSensors()
{
  int north_intensity = analogRead(north);
  int east_intensity = analogRead(east);
  int south_intensity = analogRead(south);
  int west_intensity = analogRead(west);
  
  nVal = north_intensity - n_base;
  eVal = east_intensity - e_base;
  sVal = south_intensity - s_base;
  wVal = west_intensity - w_base;

  //sensorVals[0] = {nVal, eVal, sVal, wVal};
  sensorVals[n] = nVal;
  sensorVals[e] = eVal;
  sensorVals[s] = sVal;
  sensorVals[w] = wVal;
}


void printSensors()
{
  Serial.print("north: ");
  Serial.print(nVal);
  Serial.print(", ");
  
  Serial.print("east: ");
  Serial.print(eVal);
  Serial.print(", ");
  
  Serial.print("south: ");
  Serial.print(sVal);
  Serial.print(", ");
  
  Serial.print("west: ");
  Serial.println(wVal);
}


void loop()
{
  readSensors();
  printSensors();
  determineDirection();
  Serial.println();
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
      Serial.println("head north");
    }
    else if (sensorVals[w]*1.2 >= sensorVals[e]*1.2)
    {
      Serial.println("head north-west");
    }
    else if (sensorVals[e]*1.2 >= sensorVals[w]*1.2)
    {
      Serial.println("head north-east");
    }
    else
    {
      Serial.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else if ((sensorVals[e]*.6) > sensorVals[n] && (sensorVals[e]*.6) > sensorVals[s])
  {
    if (abs(sensorVals[n]-sensorVals[s]) < 50)
    {
      Serial.println("head east");
    }
    else if (sensorVals[n]*1.2 >= sensorVals[s]*1.2)
    {
      Serial.println("head east-north-east");
    }
    else if (sensorVals[s]*1.2 >= sensorVals[n]*1.2)
    {
      Serial.println("head east-south-east");
    }
    else
    {
      Serial.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else if ((sensorVals[w]*.6) > sensorVals[n] && (sensorVals[w]*.6) > sensorVals[s])
  {
    if (abs(sensorVals[n]-sensorVals[s]) < 50)
    {
      Serial.println("head west");
    }
    else if (sensorVals[n]*1.2 >= sensorVals[s]*1.2)
    {
      Serial.println("head west-north-west");
    }
    else if (sensorVals[s]*1.2 >= sensorVals[n]*1.2)
    {
      Serial.println("head west-south-west");
    }
    else
    {
      Serial.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else if ((sensorVals[s]*.6) > sensorVals[w] && (sensorVals[s]*.6) > sensorVals[e])
  {
    if (abs(sensorVals[w]-sensorVals[e]) < 50)
    {
      Serial.println("head south");
    }
    else if (sensorVals[w]*1.2 >= sensorVals[e]*1.2)
    {
      Serial.println("head south-west");
    }
    else if (sensorVals[e]*1.2 >= sensorVals[w]*1.2)
    {
      Serial.println("head south-east");
    }
    else
    {
      Serial.println("no direction or continue moving along last direction");
      //some error
    }
  }
  else
  {
    Serial.println("got no where");
  }
}
