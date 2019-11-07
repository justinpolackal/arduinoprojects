#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//Server connect to WiFi Network
const char *ssid = "npRobot";  //Enter your wifi SSID
const char *password = "nodemcuRobot";  //Enter your wifi Password

int count = 0;
unsigned int localPort = 2000; // local port to listen for UDP packets
int led = D0;

IPAddress ServerIP(192, 168, 4, 1);
IPAddress ClientIP(192, 168, 4, 2);

// A UDP object
WiFiUDP udp;

//int buzzer = D9;
int motor[4] = {14, 12, 13, 15}; //D5,D6,D7,D8

int pwm = 1023;
int input = 0;


char packetBuffer[9];   //Where we get the UDP data

void setup()
{
  //pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  for (int i = 0; i < 4; i ++)
  {
    pinMode(motor[i], OUTPUT);
  }
  for (int i = 0; i < 4; i ++)
  {
    analogWrite(motor[i], 0);
  }

  //tone(buzzer, 100, 1000);
  Serial.begin(9600);
  Serial.println("starting");
 
  WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  WiFi.begin(ssid, password);   //Connect to access point

  Serial.println("");

  // Wait for connection
  int stat = 0;
  while (WiFi.status() != WL_CONNECTED) {
    stat = !stat;
    digitalWrite(led, stat);
    delay(500);
    Serial.print(".");
  }
  digitalWrite(led, LOW);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Start UDP
  Serial.println("Starting UDP");
  int beginRet = udp.begin(localPort);
  while (beginRet == 0 ) {  // could add a timeout here if you need it
    beginRet = udp.begin(localPort);
    delay(500);
    Serial.print("-");
  }
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  delay(1000);
  analogWrite(led, 0);
}


void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    setup();
  }

  int cb = udp.parsePacket();
  if (!cb)
  {
    //    //If serial data is recived send it to UDP
    //
    //    udp.beginPacket(ClientIP, 2000);
    //    //Send UDP requests are to port 2000
    //
    //    char a[1];
    //    a[0] = char("1"); //Serial Byte Read
    //    udp.write(a, 1); //Send one byte to ESP8266
    //    udp.endPacket();
    //    delay(2000);
  }
  else {
    if (cb > 0)
    {
      // We"ve received a UDP packet, send it to serial
      udp.read(packetBuffer, 1); // read the packet into the buffer, we are reading only one byte
      process(packetBuffer[0]);
      udp.flush();
    }
  }
}

void forward(int s)
{
  halt();

  analogWrite(motor[0], s);
  analogWrite(motor[2], s);

}

void backward(int s)
{
  halt();

  analogWrite(motor[1], s);
  analogWrite(motor[3], s);
}

void right(int s)
{
  halt();

  analogWrite(motor[0], s);
  analogWrite(motor[3], s);
}

void left(int s)
{
  halt();

  analogWrite(motor[1], s);
  analogWrite(motor[2], s);
}

void fRight(int s)
{
  halt();

  analogWrite(motor[0], s);
}

void fLeft(int s)
{
  halt();

  analogWrite(motor[2], s);
}

void bRight(int s)
{
  halt();

  analogWrite(motor[1], s);
}

void bLeft(int s)
{
  halt();

  analogWrite(motor[3], s);
}

void halt()
{
  for (int i = 0; i < 4; i++)
  {
    analogWrite(motor[i], 0);
  }
}


void process(char data)
{
  if (data == '0')
  {
    pwm = 0;
  }
  else if (data == '1')
  {
    pwm = 204;
  }
  else if (data == '2')
  {
    pwm = 409;
  }
  else if (data == '3')
  {
    pwm = 614;
  }
  else if (data == '4')
  {
    pwm = 819;
  }
  else if (data == '5')
  {
    pwm = 1024;
  }

  if (data == 'f')
  {
    forward(pwm);
  }
  else if (data == 'b')
  {
    backward(pwm);
  }
  else if (data == 'r')
  {
    right(pwm);
  }
  else if (data == 'l')
  {
    left(pwm);
  }
  else if (data == 'y')
  {
    fRight(pwm);
  }
  else if (data == 't')
  {
    fLeft(pwm);
  }
  else if (data == 'x')
  {
    bRight(pwm);
  }
  else if (data == 'z')
  {
    bLeft(pwm);
  }
  else if (data == 's')
  {
    halt();
  }
}
