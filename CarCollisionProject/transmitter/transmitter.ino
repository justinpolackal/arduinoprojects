

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>
#include <NewPing.h>


// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

char * msgs[5] = {"NSEW", "EWS", "NS", "NWS", "NES"};
int sensor1[2] = {2, 3}; //south
int sensor2[2] = {4, 5}; //east
int sensor3[2] = {6, 7}; //west


int roadWidth = 10;

NewPing sonar1(sensor1[0], sensor1[1], 200); // NewPing setup of pins and maximum distance.
NewPing sonar2(sensor2[0], sensor2[1], 200); // NewPing setup of pins and maximum distance.
NewPing sonar3(sensor3[0], sensor3[1], 200); // NewPing setup of pins and maximum distance.


unsigned int sensor1DataUs;
unsigned int sensor1Data;

unsigned int sensor2DataUs;
unsigned int sensor2Data;

unsigned int sensor3DataUs;
unsigned int sensor3Data;

char msg[10];
void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  Serial.begin(9600);
}

void loop()
{
  //msg = "";
  setDirections();
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();
  Serial.println(msg);
  Serial.println(strlen(msg));
  delay(100);

}

void readSensor()
{
  sensor1DataUs = sonar1.ping();
  sensor1Data = sensor1DataUs / US_ROUNDTRIP_CM;
  
  sensor2DataUs = sonar2.ping();
  sensor2Data = sensor2DataUs / US_ROUNDTRIP_CM;
  
  sensor3DataUs = sonar3.ping();
  sensor3Data = sensor3DataUs / US_ROUNDTRIP_CM;
}
void setDirections()
{
  msg[0] = '\0';
  readSensor();
  
  if(sensor1Data <= roadWidth-3)
  {
    strcat(msg, "S");
  }
  
  if(sensor2Data <= roadWidth-3)
  {
    strcat(msg, "E");
  }
  
  if(sensor3Data <= roadWidth-3)
  {
    strcat(msg, "W");
  }
}
