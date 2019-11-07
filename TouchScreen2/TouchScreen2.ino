#include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define TS_MINX 115
#define TS_MINY 110
#define TS_MAXX 940
#define TS_MAXY 900

#define YP A3
#define XM A2
#define YM 9
#define XP 8

#define BLACK     0x0000
#define BLUE      0x001F
#define RED       0xF800
#define GREEN     0x07E0
#define CYAN      0x07FF
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define WHITE     0xFFFF
#define DARKBLUE  0x0010
#define VIOLET    0x8888
#define GOLD      0xFEA0
#define BROWN     0xA145
#define SILVER    0xC618
#define LIME      0x07E0
#define GREY      tft.color565(64, 64, 64);

//Minimum and maximum pressure to sense the touch
//#define MINPRESSURE 10
//#define MAXPRESSURE 1000

#define relay 31
#define power 33

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

boolean buttonEnabled = true;
char currentPage;

void setup()
{
  Serial.begin(9600);

  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);

  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(3);

  currentPage = 1; // Indicates that we are at Home Screen
  drawHomescreen();  // Draws the Home Screen

}

void loop()
{
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (currentPage == 1 )
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > ts.pressureThreshhold) {

      p.x = map(p.x, TS_MAXX, TS_MINX, 320, 0);
      p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);
      Serial.print("X = "); Serial.print(p.x);
      Serial.print("\tY = "); Serial.print(p.y);
      Serial.print("\n");
      buttonEnabled = true;

      if (p.x > 30 && p.x < 80 && p.y > 280 && p.y < 470 && buttonEnabled)
      {
        RelayON();
        currentPage = 2;
      }

      else if (p.x > 30 && p.x < 80 && p.y > 20 && p.y < 230 && buttonEnabled && digitalRead(relay) == HIGH)
      {
        RelayOFF();
        currentPage = 3;
      }
//      else if (p.x > 30 && p.x < 80 && p.y > 20 && p.y < 230 && buttonEnabled && digitalRead(relay) == LOW)
//      {
//        tft.fillScreen(WHITE);
//      }

    }
  }

  if (currentPage == 2)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > ts.pressureThreshhold) {
      p.x = map(p.x, TS_MAXX, TS_MINX, 320, 0);
      p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);
      buttonEnabled = true;
      if (p.x > 30 && p.x < 300 && p.y > 20 && p.y < 470 && buttonEnabled)
      {
        tft.fillScreen(GREEN);
        drawHomescreen();
      }
    }
  }

  if (currentPage == 3)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > ts.pressureThreshhold) {
      p.x = map(p.x, TS_MAXX, TS_MINX, 320, 0);
      p.y = map(p.y, TS_MAXY, TS_MINY, 0, 480);
      buttonEnabled = true;
      if (p.x > 30 && p.x < 300 && p.y > 20 && p.y < 470 && buttonEnabled)
      {
        tft.fillScreen(GREEN);
        drawHomescreen();
      }
    }

  }
}

  void drawHomescreen()
  {
    currentPage = 1;
    tft.fillScreen(WHITE);

    tft.fillRect(20, 180, 120, 40, GREEN);
    tft.drawRect(20, 180, 120, 40, BLACK);
    tft.drawRect(19, 179, 122, 42, BLACK);
    tft.drawRect(18, 178, 124, 44, BLACK);
    tft.drawRect(17, 177, 126, 46, BLACK);
    tft.drawRect(16, 176, 128, 48, BLACK);
    tft.drawRect(15, 175, 130, 50, BLACK);
    tft.drawRect(14, 174, 132, 52, BLACK);

    tft.setCursor(32, 195);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("RELAY ON");

    tft.fillRect(170, 180, 130, 40, RED);
    tft.drawRect(170, 180, 130, 40, BLACK);
    tft.drawRect(169, 179, 132, 42, BLACK);
    tft.drawRect(168, 178, 134, 44, BLACK);
    tft.drawRect(167, 177, 136, 46, BLACK);
    tft.drawRect(166, 176, 138, 48, BLACK);
    tft.drawRect(165, 175, 140, 50, BLACK);
    tft.drawRect(164, 174, 142, 52, BLACK);

    tft.setCursor(182, 195);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("RELAY OFF");
  }


  void RelayON()
  {
    currentPage = 2;
    buttonEnabled = false;

    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    digitalWrite(relay, HIGH);

    buttonEnabled = true;

    tft.fillScreen(GREEN);
    tft.fillRect(90, 35, 130, 40, WHITE);
    tft.drawRect(90, 35, 130, 40, BLACK);
    tft.drawRect(89, 34, 132, 42, BLACK);
    tft.drawRect(88, 33, 134, 44, BLACK);
    tft.drawRect(87, 32, 136, 46, BLACK);
    tft.drawRect(86, 31, 138, 48, BLACK);
    tft.drawRect(85, 30, 140, 50, BLACK);

    tft.setCursor(115, 45);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("Go back");

    tft.setCursor(115, 115);
    tft.setTextColor(WHITE);
    tft.setTextSize(8);
    tft.print("ON");
  }

  void RelayOFF()
  {
    currentPage = 3;
    buttonEnabled = false;

    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    digitalWrite(relay, LOW);

    buttonEnabled = true;

    tft.fillScreen(RED);
    tft.fillRect(90, 35, 130, 40, WHITE);
    tft.drawRect(90, 35, 130, 40, BLACK);
    tft.drawRect(89, 34, 132, 42, BLACK);
    tft.drawRect(88, 33, 134, 44, BLACK);
    tft.drawRect(87, 32, 136, 46, BLACK);
    tft.drawRect(86, 31, 138, 48, BLACK);
    tft.drawRect(85, 30, 140, 50, BLACK);

    tft.setCursor(115, 45);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("Go back");

    tft.setCursor(100, 105);
    tft.setTextColor(WHITE);
    tft.setTextSize(8);
    tft.print("OFF");
  }

  /*        if (digitalRead(relay == HIGH))
            {      tft.setCursor(60,40);
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
             tft.print("RELAY IS ON");
            }
          if (digitalRead(relay == LOW))
            {      tft.setCursor(60,40);
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
            tft.print("RELAY IS OFF");
            }
  */

