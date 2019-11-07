#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>
//#include <cstdio>
//#include <iostream>

#define LED D5 //led pin number
// Initialize Wifi connection to the router
const char* ssid     = "lumixAsianetBB";
const char* password = "justinpolackal";
// Initialize Telegram BOT
const char BotToken[] = "514847143:AAH2BYLt5E9StzCB00Xoks7QLCzTX32WVU4";
#define CHAT_ID "486913555" // Chat ID of where you want the message to go (You can use MyIdBot to get the chat ID)

WiFiClientSecure net_ssl;
TelegramBot bot (BotToken, net_ssl);

//#include <Wire.h>

//#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd (0x3F);



// the number of the LED pin
void setup()
{
//  Wire.begin(2,0);
//  lcd.begin(16,2);// put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) {}  //Start running when the serial is open
//  delay(3000);
  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  digitalWrite(LED_BUILTIN, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("-");
    delay(500);
  }
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Starting bot...");
  bot.begin();
  Serial.println("Done");
  digitalWrite(LED_BUILTIN, HIGH);
  bot.sendMessage(CHAT_ID, "ESPtelegram BOT Started");
  pinMode(LED, OUTPUT);
}
void loop()
{
  message m = bot.getUpdates(); // Read new messages
  if (m.text.equals("ON"))
  {
    digitalWrite(LED, HIGH);
    Serial.println("ON");
    bot.sendMessage(CHAT_ID, "The Led is now ON");
  }
  else if (m.text.equals("OFF"))
  {
    digitalWrite(LED, LOW);
    Serial.println("OFF");

    bot.sendMessage(CHAT_ID, "The Led is now OFF");
  }
//  if (Serial.available())
//  {
//    int data = Serial.read();
//    bot.sendMessage(CHAT_ID, data);
//  }
}
