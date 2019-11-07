
#include <Adafruit_GFX.h>    // Adafruit's core graphics library
#include <Adafruit_TFTLCD.h> // Adafruit's hardware-specific library
#include <TouchScreen.h>
#include <EEPROM.h>
bool buttonEnabled = true;

#define YP A3  // must be an analog pin
#define XM A2  // must be an analog pin
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

//Create the touchscreen object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//Some of the tft pins
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// Optional, used to reset the display
#define LCD_RESET A4
int function;
// Define some TFT readable colour codes to human readable codes
#define BLACK   0x0000
#define BLUE    0x001F
#define DARKBLUE 0x0010
#define VIOLET 0x8888
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Create the tft object
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define MINPRESSURE 10
#define MAXPRESSURE 1000




void setup() {
  tft.reset();
  tft.begin(0x9341);// put your setup code here, to run once:
  //  Serial.begin(9600);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  tft.drawRoundRect(0, 0, 319, 240, 20, WHITE);

  tft.setCursor(100, 300);
  tft.setTextSize(3);

  tft.setTextColor(YELLOW);

  tft.setCursor(70, 50);
  tft.print("Notes");
  tft.print("Point");

  tft.setCursor(70, 120);
  tft.setTextColor(RED);
  tft.print(" Youtube!");

  tft.fillRect(60, 180, 200, 40, RED);
  tft.drawRect(60, 180, 200, 40, WHITE);
  tft.setCursor(80, 100);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println();
  tft.println();
  tft.println();
  tft.println();
  tft.setCursor(320, 165);
  tft.print("    Subscribe!");
}

void loop() {

  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint(); // put your main code here, to run repeatedly:
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);
  Serial.print("y=");
  Serial.println(p.y);
  Serial.print("x=");
  Serial.println(p.x);

  if (p.x > 235 && p.x < 292 && p.y > 37 && p.y < 177  && p.z > MINPRESSURE && p.z < MAXPRESSURE && buttonEnabled)
  {
    buttonEnabled = false;

    tft.fillRect(60, 180, 200, 40, WHITE);
    delay(100);
    tft.fillRect(60, 180, 200, 40, RED);
    tft.drawRect(60, 180, 200, 40, WHITE);

    tft.setCursor(80, 100);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println();
    tft.println();
    tft.println();
    tft.println();
    tft.setCursor(320, 165);
    tft.print("    Subscribe!");

    tft.fillScreen(BLACK);
    tft.setCursor(20, 75);
    tft.setTextColor(WHITE);
    tft.println("Thanks for");
    tft.setCursor(75, 125);
    tft.print("Subscribing!!");
  }
}

