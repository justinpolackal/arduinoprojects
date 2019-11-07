int pin = A0;
int rpin = 2;
int gpin = 3;
int bpin = 4;
void setup() {
  Serial.begin(9600);
  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(bpin, OUTPUT);
}

void loop() {
  int rb, gb, bb;
  rgb(255, 0, 0);
  delay(500);
  rb = map(analogRead(pin), 0, 1023, 255, 0);

  rgb(0, 255, 0);
  delay(500);
  gb = map(analogRead(pin), 0, 1023, 255, 0);

  rgb(0, 0, 255);
  delay(500);
  bb = map(analogRead(pin), 0, 1023, 255, 0);

  rgb(rb, gb, bb);
  Serial.print(rb);
  Serial.print(", ");
  Serial.print(gb);
  Serial.print(", ");
  Serial.println(bb);
  delay(1000);
}

void rgb(int r, int g, int b)
{
  analogWrite(rpin, r);
  analogWrite(gpin, g);
  analogWrite(bpin, b);
}

