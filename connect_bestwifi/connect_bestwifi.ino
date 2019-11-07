
#include <ESP8266WiFi.h>

struct wifi_ap
{
  char ssid      [50];
  char password  [50];
  int  siglevel      ;
};

struct wifi_ap known_aplist[10];
int known_ap_count = 0;
int led = LED_BUILTIN;

void prep_aplist()
{
  strcpy(known_aplist[0].ssid, "lumixAsianetBB");
  strcpy(known_aplist[0].password, "justinpolackal");
  known_aplist[0].siglevel = -9999;

  strcpy(known_aplist[1].ssid, "linksys");
  strcpy(known_aplist[1].password, "");
  known_aplist[1].siglevel = -9999;

  strcpy(known_aplist[2].ssid, "JioFi2_4F6C7B");
  strcpy(known_aplist[2].password, "295fmq13j2");
  known_aplist[2].siglevel = -9999;

  known_ap_count = 3;
}


void setup()
{
  pinMode(led, OUTPUT);
  prep_aplist();
  Serial.begin(115200);
  connect_wifi();
}

void loop()
{


}

bool connect_wifi()
{
  int min_sig_level = -99999;
  int selected_ap = -1;
  int ap_index = -1;
  char tempssid[50];
  

  int n = WiFi.scanNetworks();
  digitalWrite(led, HIGH);
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    //
    // Get access points and their signal strength
    //
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");

      WiFi.SSID(i).toCharArray(tempssid, 50);
      ap_index =  is_known_ap(tempssid);

      if (ap_index >= 0)
      {
        known_aplist[ap_index].siglevel = WiFi.RSSI(i);
      }

      delay(10);
    }
    //
    //Sort known ap list on descending order of signal strength
    //
    sort_aplist();
    //
    // Try connecting to access points in known_aplist array
    //
    for (int i = 0; i < known_ap_count; i++)
    {
      Serial.print(i);
      Serial.print(" SSID: ");
      Serial.print(known_aplist[i].ssid);
      Serial.print(" Signal Strength: ");
      Serial.println(known_aplist[i].siglevel);
    }

    // Connect to the best access point, whose SSID is in aplist[i]

    for (int i = 0; i < known_ap_count; i++)
    {
      WiFi.begin(known_aplist[i].ssid, known_aplist[i].password);
      int t = 0;
      while (WiFi.status() != WL_CONNECTED && t < 40 )
      {
        t++;
        delay(200);
        Serial.print("-");
      }
        
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println(known_aplist[i].ssid);
        return 1;
      }

    }
  }
  digitalWrite(led, LOW);
  return 0;
}

//
//Returns 1, if the given ssid is a known one. Otherwise returns 0
//
int is_known_ap(char * ssid)
{
  int i = 0;
  while (i < known_ap_count)
  {
    if (strcmp(ssid, known_aplist[i].ssid) == 0)
    {
      return i;
    }
    i++;
  }
  return -1;
}

// Sort known aplist in descending order of siglevel
void sort_aplist()
{
  int i = 0, j = 0, ex = 0;
  struct wifi_ap temp;

  if (known_ap_count < 2) return;

  for (i = 0; i < known_ap_count; i++)
  {
    ex = 0;
    for (j = 0; j < known_ap_count - 1; j++)
    {
      if (known_aplist[j].siglevel < known_aplist[j + 1].siglevel)
      {
        strcpy(temp.ssid, known_aplist[j].ssid);
        strcpy(temp.password, known_aplist[j].password);
        temp.siglevel = known_aplist[j].siglevel;

        strcpy(known_aplist[j].ssid, known_aplist[j + 1].ssid);
        strcpy(known_aplist[j].password, known_aplist[j + 1].password);
        known_aplist[j].siglevel = known_aplist[j + 1].siglevel;

        strcpy(known_aplist[j + 1].ssid, temp.ssid);
        strcpy(known_aplist[j + 1].password, temp.password);
        known_aplist[j + 1].siglevel = temp.siglevel;

        ex = ex + 1;
      }
    }
    if (ex == 0) return;
  }
}

