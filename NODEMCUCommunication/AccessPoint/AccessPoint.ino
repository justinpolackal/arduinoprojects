#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Nodemcu";
const char *pass = "password";
int c = 0;
unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192, 168, 4, 1);
IPAddress ClientIP(192, 168, 4, 2);

// A UDP object
WiFiUDP udp;

char packetBuffer[9];   //Where we get the UDP data

void setup()
{
  Serial.begin(9600);
  Serial.println();
  WiFi.softAP(ssid, pass);    //Create Access point

  //Start UDP
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
}

void loop()
{ 
  int cb = udp.parsePacket();
  if (!cb)
  {
    //If serial data is recived send it to UDP

    udp.beginPacket(ClientIP, 2000);
    //Send UDP requests are to port 2000

    char a[1];
    a[0] = char('1'); //Serial Byte Read
    udp.write(a, 1); //Send one byte to ESP8266
    udp.endPacket();
    delay(2000);
  }
  else {
    // We've received a UDP packet, send it to serial
    udp.read(packetBuffer, 1); // read the packet into the buffer, we are reading only one byte
    Serial.print(packetBuffer);
    delay(25);
  }
}
