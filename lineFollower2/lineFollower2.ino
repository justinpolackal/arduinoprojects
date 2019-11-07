int sensorr = 7;
int sensorl = 8;
#define lmotorfwd 18
#define lmotorbk 19
#define rmotorfwd 5
#define rmotorbk 3
void setup() {
  pinMode(sensorr, INPUT);
  pinMode(sensorl, INPUT);
  pinMode(rmotorfwd, OUTPUT);
  pinMode(rmotorbk, OUTPUT);
  pinMode(lmotorfwd, OUTPUT);
  pinMode(lmotorbk, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(digitalRead(sensorr));
  if((digitalRead(sensorr) == LOW && digitalRead(sensorl) == LOW) || (digitalRead(sensorr) == HIGH && digitalRead(sensorl) == HIGH))
  {
    motor(1, 0, 1, 0);
  }
  else if(sensor(sensorr))
  {
    motor(0, 0, 1, 0);
  }
  else if(sensor(sensorl))
  {
    motor(1, 0, 0, 0); 
  }
}

void motor(int lmforward, int lmbackward, int rmforward, int rmbackward)
{
  if (lmforward == 1)
  {
    digitalWrite(lmotorfwd, HIGH);
  }
  else 
  {
    digitalWrite(lmotorfwd, LOW);
  }

  if (lmbackward == 1)
  {
    digitalWrite(lmotorbk, HIGH);
  }
  else 
  {
    digitalWrite(lmotorbk, LOW);
  }

  if (rmforward == 1)
  {
    digitalWrite(rmotorfwd, HIGH);
  }
  else 
  {
    digitalWrite(rmotorfwd, LOW);
  }

  if (rmbackward == 1)
  {
    digitalWrite(rmotorbk, HIGH);
  }
  else 
  {
    digitalWrite(rmotorbk, LOW);
  }
}

bool sensor(int pin)
{
  if(digitalRead(pin) == HIGH)
  {
    return true;
  }
  else
   return false;
}

