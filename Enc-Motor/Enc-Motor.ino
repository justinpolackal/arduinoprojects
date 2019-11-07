int val;
int pin = 3;
int pin2 = 5;
int encoder0PinB = 10;
int encVal = 0;
int encoder0PinALast = LOW;
int sw = 2;
int n = LOW;

void setup() {
  pinMode (encoder0PinB, INPUT);
  pinMode(sw, INPUT);
  pinMode(pin, OUTPUT);
  pinMode(pin2, OUTPUT);
  Serial.begin (38400);
  Serial.setTimeout(5);
  Serial.println("Starting");
}

void loop() {
  //  if (digitalRead(sw == HIGH))
  //  {
  //    encVal = 0;
  //  }
  if (Serial.available() > 0)
  {
    turnmotor(serialReadInt(), 10, 0, 10000, 3);
  }

}
int readEncoder(int pin1, uint64_t lmin, uint64_t lmax)
{
  n = digitalRead(pin1);
  if (!n == encoder0PinALast)
  {
    encVal++;

    if (encVal < lmin)
    {
      encVal = lmin;
    }
    else if (encVal > lmax)
    {
      encVal = lmax;
    }
    //    Serial.print (encVal);
    //    Serial.println ("/");
  }
  encoder0PinALast = n;
  return encVal;
}

int intify(char d)
{
  return d - '0';
}

void turnmotor(int s, int encpin, int lmin, int lmax, int mpin)
{
  encVal = 0;
  while (readEncoder(encpin, lmin, lmax) <= s)
  {
    analogWrite(mpin, 230);
    Serial.println(readEncoder(encpin, lmin, lmax));
  }
  encVal = 0;
  analogWrite(mpin, 0);
}

int serialReadInt()
{
  if (Serial.available() > 0)
  {
    return Serial.parseInt();
  }
}


