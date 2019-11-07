int val;
//Temperature sensor is connected to analog pin 1
int tempPin=A0;
// Pin 13 has an LED connected on most Arduino boards.
//int led = 13;

void setup()
{
Serial.begin(9600);
//pinMode(led, OUTPUT);
//pinMode(tempPin,OUTPUT);
}

void loop()
{
//digitalWrite(led, HIGH);   // turn LED on  
val=analogRead(tempPin);   // read analog signal from temperature sensor
float mv=( val/1024.0)*5000;
float cel=mv/10;           // calculate temp. in celsius
float farh=(cel*9)/5+32;   // calculate temp. in fahrenheit

Serial.print("TEMPURATURE="); 
Serial.print(cel);         // Print celsius
Serial.print("*C ");
/*Serial.println();*/
/*delay(4000);*/

/* uncomment this to get tempurature in farenhite
Serial.print("TEMPURATURE=");*/
Serial.print(farh);        // print fahrenheit
Serial.print("*F");
Serial.println();          // prit newline

//digitalWrite(led, LOW);    // turn LED off
delay(500); 


}
