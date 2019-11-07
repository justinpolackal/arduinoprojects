#include <Servo.h> //include the servo library
#include <IRremote.h>

const int trigPin = 9;
const int echoPin = 8;
// defines variables
long duration;
float distance;
int receiver = 12; // Signal Pin of IR receiver to Arduino Digital Pin 11
char a[50];
char transilateIR();

IRrecv irrecv(receiver);                 //IR receiver object
decode_results results;                 //Object to store decoded data

//store potentiometer data
int pot1[100] = {};
int pot2[100] = {};
int pot3[100] = {};
int pot4[100] = {};
int pot5[100] = {};

//store delay
int saveDel[100] = {};

//the buttons - save a position/start action

//potentiometer pins
int pot1Pin = 0;
int pot2Pin = 1;
int pot3Pin = 2;
int pot4Pin = 3;
int pot5Pin = 4;

//servo pins
int servo1Pin = 2;
int servo2Pin = 3;
int servo3Pin = 4;
int servo4Pin = 5;
int servo5Pin = 6;

int led = 13;

//servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

int32_t rounds = 0; //keeps the number of loops the code had made - to store data in the correct position of the array

//temporarily stores potentiometer values
int p1 = 0;
int p2 = 1;
int p3 = 2;
int p4 = 3;
int p5 = 4;

//temporarily stores servo values
int s1 = 2;
int s2 = 3;
int s3 = 4;
int s4 = 5;
int s5 = 6;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //Attach all servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);
}

void loop() {
  Serial.println(calcDistance());
  Serial.println("looping");
  char rcv_char = ' ';
  int  input_str_len = 0;

  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    rcv_char = translateIR();
    if (rcv_char >= '0' && rcv_char <= '9') //process the ir signal
    {
      input_str_len = strlen(a);      //get current length of string
      a[input_str_len]   = rcv_char;  //assign received character to the last position in the string
      a[input_str_len + 1] = '\0';    //insert new string terminator

    }

    delay(500);
    irrecv.resume(); // receive the next value
  }
  if (rcv_char == '*') //if * pressed
  {
    //store the actual values(learning)
    pot1[rounds] = s1;
    pot2[rounds] = s2;
    pot3[rounds] = s3;
    pot4[rounds] = s4;
    pot5[rounds] = s5;
    rounds += 1; //count the number of saves
  }
  //before bressing the save key
  //take the values
  p1 = analogRead(pot1Pin);
  p2 = analogRead(pot2Pin);
  p3 = analogRead(pot3Pin);
  p4 = analogRead(pot4Pin);
  p5 = analogRead(pot5Pin);

  s1 = map(p1, 0, 1023, 0, 180);
  s2 = map(p2, 0, 1023, 0, 180);
  s3 = map(p3, 0, 1023, 0, 180);
  s4 = map(p4, 0, 1023, 0, 180);
  s5 = map(p5, 0, 1023, 0, 180);

  servo1.write(s1);
  servo2.write(s2);
  servo3.write(s3);
  servo4.write(s4);

  if (rcv_char == '!' && rounds > 0) //if ok pressed
  {
    boolean exitLoop = true;
    Serial.println(sizeof(pot1[0]));
    while (exitLoop) //after pressing the button...
    {
      Serial.println(calcDistance());
      while(calcDistance() > 25)
      {
        if (getIr() == '!') //If ok pressed again
        {
          //  reset everything
          rounds = 0;
          int pot1[100] = {};
          int pot2[100] = {};
          int pot3[100] = {};
          int pot4[100] = {};
          int pot5[100] = {};
          int saveDel[100] = {};
          exitLoop = false;
        }
      }
      for (int i = 0; i < rounds; i++) //do what the robot learned
      {
        playservo(i);
        delay(100);
//        servo1.write(pot1[i]);
//        servo2.write(pot2[i]);
//        servo3.write(pot3[i]);
//        servo4.write(pot4[i]);
//        delay(500);
        if (getIr() == '!') //If ok pressed again
        {
          //  reset everything
          rounds = 0;
          int pot1[100] = {};
          int pot2[100] = {};
          int pot3[100] = {};
          int pot4[100] = {};
          int pot5[100] = {};
          int saveDel[100] = {};
          exitLoop = false;
        }
      }
      
    }
  }
}

void playservo(int index)
{
  //servo1
  int startPos = servo1.read();
  if (startPos < pot1[index])
  {
    for (int i = startPos; i < pot1[index]; i++)
    {
      servo1.write(i);
      delay(15);
    }
  }
  else
  {
    for (int i = startPos; i > pot1[index]; i--)
    {
      servo1.write(i);
      delay(15);
    }
  }
  
  startPos = servo2.read();
  if (startPos < pot2[index])
  {
    for (int i = startPos; i < pot2[index]; i++)
    {
      servo2.write(i);
      delay(15);
    }
  }
  else
  {
    for (int i = startPos; i > pot2[index]; i--)
    {
      servo2.write(i);
      delay(15);
    }
  }
  startPos = servo3.read();
  if (startPos < pot3[index])
  {
    for (int i = startPos; i < pot3[index]; i++)
    {
      servo3.write(i);
      delay(15);
    }
  }
  else
  {
    for (int i = startPos; i > pot3[index]; i--)
    {
      servo3.write(i);
      delay(15);
    }
  }
  startPos = servo4.read();
  if (startPos < pot4[index])
  {
    for (int i = startPos; i < pot4[index]; i++)
    {
      servo4.write(i);
      delay(15);
    }
  }
  else
  {
    for (int i = startPos; i > pot4[index]; i--)
    {
      servo4.write(i);
      delay(15);
    }
  }
}
char translateIR() // Takes action based on IR code received
{
  int i = 0;
  char rcv_char = ' ';
  switch (results.value)

  {

    case 0x511DBB: Serial.println(" FORWARD"); rcv_char = 'F'; break;
    case 0x52A3D41F: Serial.println(" LEFT");  rcv_char = 'L'; break;
    case 0xD7E84B1B: Serial.println(" -OK-");  rcv_char = '!'; break;
    case 0x20FE4DBB: Serial.println(" RIGHT"); rcv_char = 'R'; break;
    case 0xA3C8EDDB: Serial.println(" REVERSE"); rcv_char = 'B'; break;
    case 0xC101E57B: Serial.println(" 1");  rcv_char = '1'; break;
    case 0x97483BFB: Serial.println(" 2");  rcv_char = '2'; break;
    case 0xF0C41643: Serial.println(" 3");  rcv_char = '3'; break;
    case 0x9716BE3F: Serial.println(" 4");  rcv_char = '4'; break;
    case 0x3D9AE3F7: Serial.println(" 5");  rcv_char = '5'; break;
    case 0x6182021B: Serial.println(" 6");  rcv_char = '6'; break;
    case 0x8C22657B: Serial.println(" 7");  rcv_char = '7'; break;
    case 0x488F3CBB: Serial.println(" 8");  rcv_char = '8'; break;
    case 0x449E79F: Serial.println(" 9");   rcv_char = '9'; break;
    case 0x32C6FDF7: Serial.println(" *");  rcv_char = '*'; break;
    case 0x1BC0157B: Serial.println(" 0");  rcv_char = '0'; break;
    case 0x3EC3FC1B: Serial.println(" #");  rcv_char = '#'; break;
    case 0xFFFFFFFF: Serial.println(" REPEAT"); rcv_char = '-'; digitalWrite(led, HIGH); delay(100); digitalWrite(led, LOW); break;

    default:
      Serial.println(" other button   "); Serial.println(results.value, HEX);
      digitalWrite(led, HIGH);
      delay(100);
      digitalWrite(led, LOW);
      break;

  }// End Case

  return rcv_char;
}
char getIr()
{
  char rcv_char = ' ';
  int  input_str_len = 0;

  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    rcv_char = translateIR();
    if (rcv_char >= '0' && rcv_char <= '9')
    {
      input_str_len = strlen(a);      //get current length of string
      a[input_str_len]   = rcv_char;  //assign received character to the last position in the string
      a[input_str_len + 1] = '\0';    //insert new string terminator

    }

    delay(500);
    irrecv.resume(); // receive the next value
  }
  return rcv_char;
}
//10 - echo, 11 - trig
int calcDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.0343 / 2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);

  return distance;
}
