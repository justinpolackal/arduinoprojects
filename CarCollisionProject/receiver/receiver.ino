#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;
// Set buffer to size of expected message
uint8_t buf[32] = {};
uint8_t buflen = sizeof(buf);
uint8_t collissionDirections[10] = {};

/*
   Directions:
   N - North
   X - North East
   E - East
   Y - South East
   S - South
   Z - South West
   W - West
   P - North West
*/

char directions[17] = {'N', 'X', 'E', 'Y', 'S', 'Z', 'W', 'P', 'N', 'X', 'E', 'Y', 'S', 'Z', 'W', 'P'};
enum displayDirections {LFTDOWN, LFT, LFTUP, FRONT, RGHTUP, RGHT, RGHTDOWN};
enum displayDirections collissionDirectionsDisplay[10];

char ourDirection;
char ourLocation;

int ledLeft = 2;
int ledRight = 3;
int ledFront = 4;
void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  // Setup Serial Monitor
  Serial.begin(9600);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(ledFront, OUTPUT);
}

void loop()
{
  char *collissionData;
  char *sensorData = receive();
  ourDirection = getOurDirection();
  ourLocation = getOurLocation(ourDirection);

  if (strlen(sensorData) > 0)
  {
    //Serial.print(sensorData);

    collissionData = findCollisionDirection(sensorData, ourDirection, ourLocation);
    int k = displayCollisionData(collissionData, ourLocation);
    Serial.println(k);
    //Serial.println(collissionDirectionsDisplay[0]);
    // Perform necessary action on the possible list of collission direction codes
    // E.g: Display them, fire a buzzer, etc.
    //delay(500);

  }
}

/*
   Input Our Direction, Location and the string of
   location codes where vehicles are found.

   Returns an array of direction codes, which may result in a collission.
*/

char *findCollisionDirection(char sensorData[], char ourDirection, char ourLocation)
{
  int j = 0;
  for (int i = 0; i < strlen(sensorData); i++)
  {
    //Serial.println(sensorData[i]);
    if ((char)sensorData[i] != (char)ourLocation)
    {
      collissionDirections[j] = sensorData[i];
      j++;
    }


  }
  //Assign location codes to collissionDirections
  return collissionDirections;
}

int getCurrentLocationIndex(char ourLocation)
{
  int index = 0;
  switch (ourLocation)
  {
    case 'N': index = 0; break;
    case 'X': index = 1; break;
    case 'E': index = 2; break;
    case 'Y': index = 3; break;
    case 'S': index = 4; break;
    case 'Z': index = 5; break;
    case 'W': index = 6; break;
    case 'P': index = 7; break;
    default: index = 8;
  }
  return index;
}

int displayCollisionData(char *collissionData, char ourLocation)
{
  int i = 0;
  int k = 0;
  int l = 0;
  i = getCurrentLocationIndex(ourLocation);
  for (int j = 0; j < strlen(collissionData); j++)
  {
    //k = getCurrentLocationIndex(collissionData[j]);
    //l = k-i;
    if (char(directions[i + 1]) == char(collissionData[j]))
    {
      collissionDirections[k] = LFTDOWN;
      Serial.println("LEFTDOWN");
      k++;
    }
    if (char(directions[i + 2]) == char(collissionData[j]))
    {
      collissionDirections[k] = LFT;
      Serial.println("LEFT");
      k++;
      digitalWrite(ledLeft, HIGH);
    }
    if (char(directions[i + 3]) == char(collissionData[j]))
    {
      collissionDirections[k] = LFTUP;
      Serial.println("LEFTUP");
      k++;
    }
    if ((directions[i + 4]) == collissionData[j])
    {
      collissionDirections[k] = FRONT;
      k++;
      Serial.println("FRONT");
      digitalWrite(ledFront, HIGH);
    }
    if ((directions[i + 5]) == collissionData[j])
    {
      collissionDirections[k] = RGHTUP;
      k++;
      Serial.println("RIGHTUP");
    }
    if ((directions[i + 6]) == collissionData[j])
    {
      collissionDirections[k] = RGHT;
      k++;
      Serial.println("RIGHT");
      digitalWrite(ledRight, HIGH);
    }
    if ((directions[i + 7]) == collissionData[j])
    {
      collissionDirections[k] = RGHTDOWN;
      k++;
      Serial.println("RIGHTDOWN");
    }
  }
  return k;
}

void turnOffLEDs()
{
  digitalWrite(ledLeft, LOW);
  digitalWrite(ledRight, LOW);
  digitalWrite(ledFront, LOW);
}

char *receive()
{
  buf[0] = '\0';
  buflen = sizeof(buf);
  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen))
  {
    buf[buflen] = '\0';
    // Message received with valid checksum
    Serial.print("Message Received: ");
    Serial.println((char*)buf);
    Serial.println(buflen);

  }
  return buf;

}

/*
   Get our direction - direction of approaching the junction
*/
char getOurDirection()
{
  return 'N';
}

/*
   Given our current direction, calculate our location w.r.t the junction.
*/
char getOurLocation(char ourDirection)
{
  char loc = '0';
  if (ourDirection == 'N')
    loc = 'S';
  if (ourDirection == 'S')
    loc = 'N';
  if (ourDirection == 'E')
    loc = 'W';
  if (ourDirection == 'W')
    loc = 'E';

  if (ourDirection == 'X')
    loc = 'Z';
  if (ourDirection == 'Y')
    loc = 'P';
  if (ourDirection == 'Z')
    loc = 'X';
  if (ourDirection == 'P')
    loc = 'Y';

  return loc;
}
