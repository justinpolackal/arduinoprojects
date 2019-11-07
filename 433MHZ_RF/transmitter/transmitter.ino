#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;
char* words[10] = {"hello,"," how"," are"," you?","1","2","3","4","5 "};
int i;
void setup()
{
    // Initialize ASK Object
    pinMode(13,OUTPUT);
    rf_driver.init();
    Serial.begin(9600);
}
 
void loop()
{   
    Serial.println(words[i]);
    rf_driver.send((uint8_t *)words[i], strlen(words[i]));
    rf_driver.waitPacketSent();
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    i++;
    if(i == 9)
    {
      i = 0;
    }
}
