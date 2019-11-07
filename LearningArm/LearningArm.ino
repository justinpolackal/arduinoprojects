#include <Servo.h> //include the servo library

//store potentiometer data
int pot1[100] = {};
int pot2[100] = {};
int pot3[100] = {};
int pot4[100] = {};
int pot5[100] = {};

//store delay
int saveDel[100] = {};

//the buttons - save a position/start action
int saveButton = 7;
unsigned long lsaveDebTime2 = 0;
unsigned long saveDebCount = 0;
int saveButtonState = LOW;
int lsaveButtonState = LOW;
unsigned long lsaveDebTime = 0;
unsigned long saveDebDel = 100;
int startButton = 8;
int startButtonState;
int lstartButtonState = LOW;
unsigned long lstartDebTime = 0;
unsigned long startDebDel = 50;

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
  //initializing pins
  pinMode(saveButton, INPUT);
  pinMode(startButton, INPUT);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  servo5.attach(servo5Pin);
}

void loop() {

  while (debounceSave() == LOW) //before pressing the save button
  {
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
    servo5.write(s5);

    if (debounceStart() == HIGH) //The code that does the main action is here
    {
      Serial.println(sizeof(pot1[0]));
      while (debounceStart() == HIGH) //wait until the button is  released
      {
        Serial.println("starthigh");
      }
      while (debounceStart() == LOW) //after pressing the button...
      {
        for (int i = 0; i < rounds; i++) //do what the robot learned
        {
          servo1.write(pot1[i]);
          servo2.write(pot2[i]);
          servo3.write(pot3[i]);
          servo4.write(pot4[i]);
          servo5.write(pot5[i]);
          delay(saveDel[rounds]);
          Serial.println("working");
        }
      }
      while (debounceStart() == HIGH) //if pressed again
      {

      }
      //reset everything
      rounds = 0;
      int pot1[100] = {};
      int pot2[100] = {};
      int pot3[100] = {};
      int pot4[100] = {};
      int pot5[100] = {};
      int saveDel[100] = {};
      break;
    }
  }
  int startT = millis(); //start counter
  while (debounceSave() == HIGH) //while pressing and holding
  {
  }
  int endT = millis();
  int del = endT - startT; //calculate the press and hold time

  //store the actual values(learning)
  pot1[rounds] = s1;
  pot2[rounds] = s2;
  pot3[rounds] = s3;
  pot4[rounds] = s4;
  pot5[rounds] = s5;

  saveDel[rounds] = del; //store the delay before each action

  rounds += 1; //count the number of loops

  //  if (debounceStart() == HIGH) //The code that does the main action is here
  //  {
  //    Serial.println(sizeof(pot1[0]));
  //    while (debounceStart() == HIGH) //wait until the button is  released
  //    {
  //      Serial.println("starthigh");
  //    }
  //    while (debounceStart() == LOW) //after pressing the button...
  //    {
  //      for (int i = 0; i < rounds; i++) //do what the robot learned
  //      {
  //        servo1.write(pot1[i]);
  //        servo2.write(pot2[i]);
  //        servo3.write(pot3[i]);
  //        servo4.write(pot4[i]);
  //        servo5.write(pot5[i]);
  //        delay(saveDel[rounds]);
  //        Serial.println("working");
  //      }
  //    }
  //    while (debounceStart() == HIGH) //if pressed again
  //    {
  //
  //    }
  //    //reset everything
  //    rounds = 0;
  //    int pot1[100] = {};
  //    int pot2[100] = {};
  //    int pot3[100] = {};
  //    int pot4[100] = {};
  //    int pot5[100] = {};
  //    int saveDel[100] = {};
  //  }
}

bool debounceSave()
{
  int reading = digitalRead(saveButton);
  Serial.print("reading:");
  Serial.print(reading);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lsaveButtonState) {
    // reset the debouncing timer
    lsaveDebTime = millis();
  }

  Serial.print(" lsavedebtime:");
  Serial.print(lsaveDebTime);


  if ((millis() - lsaveDebTime) > saveDebDel) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != saveButtonState) {
      saveButtonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (saveButtonState == HIGH) {

      }
    }
  }
  Serial.print(" savebuttonstate:");
  Serial.println(saveButtonState);
  lsaveButtonState = reading;
  Serial.print(" lsavebuttonstate:");
  Serial.print(lsaveButtonState);
  return saveButtonState;
}

bool debounceStart()
{
  return LOW;
}

