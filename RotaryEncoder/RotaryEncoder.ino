int val;
int pin = 3;
int pin2 = 5;
int encoder0PinA = 9;
int encoder0PinB = 10;
int encVal = 0;
int encoder0PinALast = LOW;
int sw = 2;
int n = LOW;

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode(sw, INPUT);
  pinMode(pin, OUTPUT);
  pinMode(pin2, OUTPUT);
  Serial.begin (9600);
}

void loop() {
  //  if (digitalRead(sw == HIGH))
  //  {
  //    encVal = 0;
  //  }
  if(Serial.available())
  {
    if(Serial.read() == '1')
    {
      analogWrite(pin, 150);
      analogWrite(pin2, 0);
    }
    else{
      analogWrite(pin, 0);
      analogWrite(pin2, 150);
    }
  }
  Serial.println(abs(readEncoder(9, 10, 0, 255)));
}
int readEncoder(int pin1, int pin2, int lmin, int lmax)
{
  n = digitalRead(pin1);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(pin2) == LOW) {
      encVal--;
    } else {
      encVal++;
    }
    if (encVal < lmin)
    {
      encVal = lmin;
    }
    else if (encVal > lmax)
    {
      encVal = lmax;
    }
    Serial.print (encVal);
    Serial.println ("/");
  }
  encoder0PinALast = n;
  return encVal;
}

