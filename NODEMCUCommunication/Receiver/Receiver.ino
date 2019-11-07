#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Nodemcu";
const char *pass = "password"; 

unsigned int localPort = 2000; // local port to check for UDP packets from the transmitter

IPAddress ServerIP(192,168,4,1);
IPAddress ClientIP(192,168,4,2);

// A UDP object
WiFiUDP udp;

char packetBuffer[9];   //Where we get the UDP data
void setup()
{
    Serial.begin(9600);
    Serial.println();

    WiFi.begin(ssid, pass);   //Connect to access point
  
    Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    
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

//      udp.beginPacket(ServerIP, 2000);  
//        
//      char a[1];
//      a[0]=char('1'); 
//      udp.write(a,1); 
//      udp.endPacket();
//        }
    }
    else {
      // We've received a UDP packet from ap
      udp.read(packetBuffer, 1); // read the packet into the buffer
      Serial.print(packetBuffer);
      delay(25);
    }
}
//=======================================================================
