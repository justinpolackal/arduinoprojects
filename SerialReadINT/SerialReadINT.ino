byte b;
long i = -1;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  while(Serial.available() == 0) {}
  Serial.println(serialReadInt()); 
}

int serialReadInt()
{
  if (Serial.available() > 0)
  {
    return Serial.parseInt();
  }
}

