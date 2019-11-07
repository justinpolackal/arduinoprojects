//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd ( 0x27 , 2 , 1 , 0 , 4 , 5 , 6 , 7 , 3 , POSITIVE);   //LCD object

void setup()
{
  lcd.begin(16, 2);                    // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("Ywrobot Arduino!");
  
}


void loop()
{
}
