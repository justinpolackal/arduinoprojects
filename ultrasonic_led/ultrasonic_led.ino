const int trigPin = 9;
const int echoPin = 10;
int ledPin = 11;
float avg_dist = 0.00; 
float skip_threshhold = 50.0;
int skipped = 0;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.print(readDistance());
  Serial.println(",");
}


int readDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  float distance = duration * 0.0343 / 2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);

  return duration;
}

float distAverage()
{
  float sensorVal = -1.0;
  float difference = 0.0;
  while(sensorVal > 400 || sensorVal < 0)
  {
    sensorVal = readDistance(); 
  }
  Serial.println(sensorVal);
  if(avg_dist < 0.1)
  {
    avg_dist = sensorVal;
    return avg_dist;
  }
  difference = avg_dist-sensorVal;
  if(abs(difference) < skip_threshhold)
  {
    avg_dist = ((avg_dist+sensorVal)/2.0);
    return avg_dist;
  }
  else
  {
    if(skipped >= 3)
    {
      avg_dist = sensorVal;
      skipped = 0;
      return avg_dist;
    }
    else
    {
      skipped++;
      Serial.print("Skipped:");
      Serial.println(sensorVal);
      return avg_dist;
    }
  }
  
  
}

