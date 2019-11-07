#include <Adafruit_GFX.h>    // Adafruit's core graphics library
#include <Adafruit_TFTLCD.h> // Adafruit's hardware-specific library
#include <TouchScreen.h>     //Touchscreen library
#include <Fonts/Org_01.h>    //Include a different font
#include <EEPROM.h>         //Include the EEPROM library to score the highscore
#include <Fonts/FreeSevenSegNumFontBitmaps.h>

#define YP A3  // must be an analog pin
#define XM A2  // must be an analog pin
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 100
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

//Create the touchscreen object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  //(data,data,data,data,sensitivity);

//Some of the tft pins
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// Optional, used to reset the display
#define LCD_RESET A4

//Create the tft object
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Define some TFT readable colour codes to human readable names
#define BLACK   0x0000
const int BLUE = tft.color565(50, 50, 255);
const int LIGHTBLUE = tft.color565(70, 149, 255);
#define DARKBLUE 0x0010
#define VIOLET 0x8888
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY   tft.color565(64, 64, 64);
#define GOLD 0xFEA0
#define BROWN 0xA145
#define SILVER 0xC618
#define LIME 0x07E0

//Minimum and maximum pressure to sense the touch
#define MINPRESSURE 10
#define MAXPRESSURE 1000

void setup()
{
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.fillRect(0, 225, 320, 230, tft.color565(80, 170, 255));

}

void loop()
{
  int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
  x1 = 200;
  y1 = 230;
  x2 = 240;
  y2 = 230;
  x3 = 160;
  y3 = 230;
  x4 = 120;
  y4 = 230;
  x5 = 80;
  y5 = 230;
  for (int i = 20; i > 0; i--)
  {
    y1 -= (i - 1);
    y2 -= (i - 2);
    y3 -= (i - 0);
    y4 -= (i - 1);
    y5 -= (i - 2);
    tft.fillCircle(x1, y1, 10, tft.color565(60, 139, 255));
    tft.fillCircle(x2, y2, 10, tft.color565(255, 150, 0));
    tft.fillCircle(x3, y3, 10, tft.color565(255, 255, 0));
    tft.fillCircle(x4, y4, 10, tft.color565(60, 139, 255));
    tft.fillCircle(x5, y5, 10, tft.color565(255, 150, 0));
//    delay(-10);
    tft.fillCircle(x1, y1, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x2, y2, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x3, y3, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x4, y4, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x5, y5, 10, tft.color565(0, 0, 0));


  }
  for (int i = 0; i < 20; i++)
  {
    y1 += (i - 1);
    y2 += (i - 2);
    y3 += (i - 0);
    y4 += (i - 1);
    y5 += (i - 2);
    tft.fillCircle(x1, y1, 10, tft.color565(60, 139, 255));
    tft.fillCircle(x2, y2, 10, tft.color565(255, 150, 0));
    tft.fillCircle(x3, y3, 10, tft.color565(255, 255, 0));
    tft.fillCircle(x4, y4, 10, tft.color565(60, 139, 255));
    tft.fillCircle(x5, y5, 10, tft.color565(255, 150, 0));
//    delay(-10);
    tft.fillCircle(x1, y1, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x2, y2, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x3, y3, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x4, y4, 10, tft.color565(0, 0, 0));
    tft.fillCircle(x5, y5, 10, tft.color565(0, 0, 0));
  }
}

