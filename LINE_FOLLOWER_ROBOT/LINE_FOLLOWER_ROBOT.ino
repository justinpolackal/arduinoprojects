//Define motor pins
#define M1A 3
#define M1B 5
#define M2A 6
#define M2B 9

//Speed
#define S 255
#define SO 0
#define SLIGHT 150

//Define ir sensor pins
#define IR5 10
#define IR4 11
#define IR3 12
#define IR2 14
#define IR1 15

#define STATEPIN 2

//The array to store current and history of sensor data
int Signal[5][5] = {
  {1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1}
};
//Array variables and constants
int arrayRowPos = 0;
#define MAXROW 4
int surfaceType = 0;

//Define two types of surface (Black line on white surface, White line on black surface)
#define BLACKONWHITE 1
#define WHITEONBLACK 0

#define PATH_LINE 0

//Define some kind of lines
#define LINE 0
#define RIGHT90 1
#define LEFT90 2
#define TJUNCT 3
#define XJUNCT 4

int state = 0;

void setup()
{
  Serial.begin(115200);

  //Set the pinmodes of all the pins
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);
  pinMode(STATEPIN, INPUT);

  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  //Identify surface type
  surfaceType = idfySurfaceType();
  Serial.println(surfaceType);
}

void loop()
{
  readIrArray();

  if (Signal[0][0] == 0 && Signal[0][1] == 0 && Signal[0][2] == 0)
  {
    left();
  }
  else if (Signal[0][0] == 0 && Signal[0][1] == 0)
  {
    left();
  }
  else if (Signal[0][1] == 0 && Signal[0][2] == 0)
  {
    left();
  }
  else
  {
    if (Signal[0][0] == 0)
    { Stop();
      left();
    }
    if (Signal[0][1] == 0)
    {
      Stop();
      slightleft();
    }
    if (Signal[0][2] == 0)
    {
      Stop();
      forward();
    }
  }

  if (Signal[0][4] == 0 && Signal[0][3] == 0 && Signal[0][2] == 0)
  {
    right();
  }
  else if (Signal[0][4] == 0 && Signal[0][3] == 0)
  {
    right();
  }
  else if (Signal[0][4] == 1 && Signal[0][3] == 0 && Signal[0][2] == 0)
  {
    right();
  }
  else
  {
    if (Signal[0][3] == 0)
    {
      Stop();
      slightright();
    }
    if (Signal[0][4] == 0)
    {
      Stop();
      right();
    }
  }
}

int readIrArray() //Store it into the array
{
  int dat[5] = {1, 1, 1, 1, 1};

  dat[0] = readIr(IR1);
  dat[1] = readIr(IR2);
  dat[2] = readIr(IR3);
  dat[3] = readIr(IR4);
  dat[4] = readIr(IR5);

  if (dat[0] == Signal[0][0] && dat[1] == Signal[0][1] && dat[2] == Signal[0][2] && dat[3] == Signal[0][3] && dat[4] == Signal[0][4]) //Reject the new value if same as the latast one before
  {
    return;
  }

  for (int i = MAXROW; i > 0; i--)
  {
    for (int j = 0; j < MAXROW + 1; j++)
    {
      Signal[i][j] = Signal[i - 1][j];
    }
  }
  //Insert data to the array
  Signal[0][0] = dat[0];
  Signal[0][1] = dat[1];
  Signal[0][2] = dat[2];
  Signal[0][3] = dat[3];
  Signal[0][4] = dat[4];

  for (int i = 0; i <= MAXROW; i++)
  {
    Serial.println();
    for (int j = 0; j <= MAXROW; j++)
    {
      Serial.print(Signal[i][j]);
    }
  }
  return 0;
}

int readIr(int irNo) //Read data from IR sensors
{
  int val = digitalRead(irNo);
  if (surfaceType == BLACKONWHITE)
  {
    if (val == LOW)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  if (surfaceType == WHITEONBLACK)
  {
    if (val == HIGH)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int idfySurfaceType() //Identify surface type by checking wich colour is readed from majority of sensors (White, Black)
{
  int white = 0;
  int black = 0;

  if (digitalRead(IR1) == LOW)
    white++;
  else
    black++;

  if (digitalRead(IR2) == LOW)
    white++;
  else
    black++;

  if (digitalRead(IR3) == LOW)
    white++;
  else
    black++;

  if (digitalRead(IR4) == LOW)
    white++;
  else
    black++;

  if (digitalRead(IR5) == LOW)
    white++;
  else
    black++;

  //Check majority
  if (white > black)
    return BLACKONWHITE;

  else
    return WHITEONBLACK;
}

void forward()
{
  analogWrite(M1A, S); //front
  analogWrite(M2A, S);
}

void backward()
{
  analogWrite(M1B, S); //back
  analogWrite(M2B, S);
}

void Stop()
{
  analogWrite(M1A, SO);
  analogWrite(M2A, SO);

  analogWrite(M1B, SO);
  analogWrite(M2B, SO);
}

void slightright()
{
  analogWrite(M1A, S);
  analogWrite(M2A, SLIGHT); //slight right
}

void right()
{
  analogWrite(M1A, S);
  //  analogWrite(M2B, s); //right
}

void sharpright()
{
  analogWrite(M1A, S);
  analogWrite(M2B, S); //right
}

void slightleft()
{
  analogWrite(M1A, SLIGHT); //slight left
  analogWrite(M2A, S);
}

void left()
{
  analogWrite(M1B, SO); //left
  analogWrite(M2A, S);
}

void sharpleft()
{
  analogWrite(M1B, S); //left
  analogWrite(M2A, S);
}

