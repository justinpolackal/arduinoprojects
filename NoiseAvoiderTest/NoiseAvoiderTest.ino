const int trigPin = 9;
const int echoPin = 10;
int avg_dist = 0;
int skip_threshhold = 10;
int skipped = 0;
int skipped_avg = 0;
int data[382] = {
  
};

long lastmillis = 0;
int valueCount = 0;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  //  Serial.print();
  initializeFilter();
  for (int i = 0; i < 381; i++)
  {
    Serial.print(data[i]);
    Serial.print(", ");
    Serial.println(distAverage(data[i]));
  }
}

void loop() {

}


int readSensorVal()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  //  int distance = (duration * 0.0343);
  //  distance = distance >> 1;
  return duration;
}

int distAverage(int sensorVal)
{
  //  int sensorVal = -1;
  int difference = 0;
  int return_val = 0;
  //  sensorVal = readSensorVal();
  difference = (avg_dist - sensorVal);
  if (abs(difference) < skip_threshhold)
  {
    skipped = 0;
    avg_dist = ((avg_dist + sensorVal) >> 1);
    //    printValues(sensorVal, avg_dist, 0);
    return_val = avg_dist;
  }
  else
  {
    if (skipped >= 3)
    {
      avg_dist = skipped_avg;
      skipped = 0;
      //      printValues(sensorVal, avg_dist, 0);
      return_val = avg_dist;
    }
    else
    {
      skipped++;
      if (skipped_avg == 0)
      {
        skipped_avg = sensorVal;
      }
      else
        skipped_avg = ((skipped_avg + sensorVal) >> 1);

      //      printValues(sensorVal, avg_dist, sensorVal);
      return_val = avg_dist;
    }
  }
  return return_val;
}

void printValues(int sensor, int average, int skipped)
{
  Serial.println();
  Serial.print(sensor);
  Serial.print(", ");
  Serial.print(average);
  Serial.print(", ");
  if (skipped != 0)
    Serial.print(skipped);
}

void initializeFilter()
{
  avg_dist = readSensorVal();
}

