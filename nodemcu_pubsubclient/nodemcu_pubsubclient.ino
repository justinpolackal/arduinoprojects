
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "lumixAsianetBB";
const char* password = "justinpolackal";
const char* mqtt_server = "192.168.2.106";

char strpub[50];

int led1state;
int led2state;
int led3state;
int exfanstate;

int led1pin = D1;
int led2pin = D2;
int led3pin = D3;
int exfanpin = D5;

long millicount;

const int pubinterval = 500;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
    digitalWrite(LED_BUILTIN,LOW);
  }
  digitalWrite(LED_BUILTIN,HIGH);

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());
  Serial.flush();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      client.subscribe("test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(led1pin, OUTPUT);
  pinMode(led2pin, OUTPUT);
  pinMode(led3pin, OUTPUT);
  pinMode(exfanpin, OUTPUT);
  digitalWrite(led1pin, HIGH);
  digitalWrite(led2pin, HIGH);
  digitalWrite(led3pin, HIGH);
  digitalWrite(exfanpin,HIGH);
  Serial.begin(115200);
  led1state = 0;
  led2state = 0;
  led3state = 0;
  exfanstate = 0;
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  millicount = millis();
}

void loop() {
  if ((millis() - millicount) > pubinterval)
  {
   client.publish("test", "test");
   millicount = millis();
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


}


