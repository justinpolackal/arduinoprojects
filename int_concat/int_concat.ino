int a, b = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  
  
  int x = concat(59, 12); // put your main code here, to run repeatedly:
  Serial.println(x);
  delay(500);

}

int concat(int x, int y) {
  int temp = y;
  while (y != 0) {
    x *= 10;
    y /= 10;
  }
  return x + temp;
}

