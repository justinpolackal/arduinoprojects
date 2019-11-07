#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "npRobot";
const char *pass = "nodemcuRobot";

char data = ' ';
unsigned int localPort = 2000; // local port to check for UDP packets from the transmitter
int potVal = 0;

IPAddress ServerIP(192, 168, 4, 1);
IPAddress ClientIP(192, 168, 4, 2);

// A UDP object
WiFiUDP udp;

char packetBuffer[9];   //Where we get the UDP data

int buttonUp = D4;
int buttonDown = D3;
int buttonLeft = D2;
int buttonRight = D1;
int buttonP = D6;
int buttonM = D5;

char buttonPressed = 'n';
char sp = '5';
void setup()
{
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);

  pinMode(buttonP, INPUT_PULLUP);
  pinMode(buttonM, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  delay(1000);

  Serial.println("Waiting for client to connect");
  while (wifi_softap_get_station_num() == 0)
  {
    delay(200);
  }

  Serial.println("Client connected!");
  Serial.println("Starting UDP");

  int beginRet = udp.begin(localPort);

  while (beginRet == 0 ) {  // could add a timeout here if you need it
    beginRet = udp.begin(localPort);
    delay(500);
    Serial.print(".");
  }

  Serial.print("Local port: ");
  Serial.println(udp.localPort());

}
void loop()
{
  if (wifi_softap_get_station_num() == 0)
    setup();

  char chk = Button();
  if (data != chk)
  {
    Serial.println(wifi_softap_get_station_num());
    data = chk;
    Serial.println(data);

    if (data != '' and data != ' ')
    {
      int cb = udp.parsePacket();
      if (!cb)
      {

        udp.beginPacket(ClientIP, 2000);

        char a[1];
        a[0] = char(data);
        Serial.println(data);

        udp.write(a, 1);
        udp.endPacket();
        delay(3);
      }
    }
  }

  delay(20);
}

char Button()
{
  if (buttonPressed == 'n')
  {
    if (digitalRead(buttonP) == 0 && digitalRead(buttonM) == 1 && sp < '5')
      buttonPressed = 'p';
    else if (digitalRead(buttonM) == 0 && digitalRead(buttonP) == 1 && sp > '0')
      buttonPressed = 'm';
  }

  if (buttonPressed == 'p' && digitalRead(buttonP) == 1 )
  {
    buttonPressed = 'n';
    sp = sp + 1;
    return sp;
  }
  else if (buttonPressed == 'm' && digitalRead(buttonM) == 1 )
  {
    buttonPressed = 'n';
    sp = sp - 1;
    return sp;
  }

  char returnVal;

  if (digitalRead(buttonUp) == 0)
    if (digitalRead(buttonDown) == 1)
      if (digitalRead(buttonLeft) == 1)
        if (digitalRead(buttonRight) == 1)
          returnVal = 'f';

  if (digitalRead(buttonUp) == 1)
    if (digitalRead(buttonDown) == 0)
      if (digitalRead(buttonLeft) == 1)
        if (digitalRead(buttonRight) == 1)
          returnVal = 'b';

  if (digitalRead(buttonUp) == 1)
    if (digitalRead(buttonDown) == 1)
      if (digitalRead(buttonLeft) == 0)
        if (digitalRead(buttonRight) == 1)
          returnVal = 'l';

  if (digitalRead(buttonUp) == 1)
    if (digitalRead(buttonDown) == 1)
      if (digitalRead(buttonLeft) == 1)
        if (digitalRead(buttonRight) == 0)
          returnVal = 'r';

  if (digitalRead(buttonUp) == 0)
    if (digitalRead(buttonDown) == 1)
      if (digitalRead(buttonLeft) == 1)
        if (digitalRead(buttonRight) == 0)
          returnVal = 'y';

  if (digitalRead(buttonUp) == 0)
    if (digitalRead(buttonDown) == 1)
      if (digitalRead(buttonLeft) == 0)
        if (digitalRead(buttonRight) == 1)
          returnVal = 't';

  if (digitalRead(buttonUp) == 1)
    if (digitalRead(buttonDown) == 0)
      if (digitalRead(buttonLeft) == 0)
        if (digitalRead(buttonRight) == 1)
          returnVal = 'z';

  if (digitalRead(buttonUp) == 1)
    if (digitalRead(buttonDown) == 0)
      if (digitalRead(buttonLeft) == 1)
        if (digitalRead(buttonRight) == 0)
          returnVal = 'x';

  if (digitalRead(buttonUp) == 1)
    if (digitalRead(buttonDown) == 1)
      if (digitalRead(buttonLeft) == 1)
        if (digitalRead(buttonRight) == 1)
        {
          returnVal = 's';
        }
  return returnVal;


}
