#include <Adafruit_GFX.h>    // Adafruit's core graphics library
#include <Adafruit_TFTLCD.h> // Adafruit's hardware-specific library
#include <TouchScreen.h>     //Touchscreen library

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

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Define some TFT readable colour codes to human readable names
#define BLACK   0x0000
const int BLUE = tft.color565(50, 50, 255);
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

void setup() {
  tft.reset();
  tft.begin(tft.readID());// put your setup code here, to run once:
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  Serial.begin(9600);

}

void loop() {

  for (uint16_t j = 0; j <= tft.width() + 1; j++ ){
    
    float volt;
    float signalY = map(analogRead(A5),  0,  4095,  tft.height() - 1,  1   ) ;

    tft.drawFastVLine( j,  46, tft.height(), BLACK);
//    tft.drawFastHLine(j, 180.00, 1, RED);
//    tft.drawFastHLine(j, 195.00, 1, WHITE);

    tft.drawPixel(j, signalY, WHITE);
  }
}
