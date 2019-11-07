#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <Servo.h>

int receiver = 12; // Signal Pin of IR receiver to Arduino Digital Pin 11
int backlightpin = 6;
int brightness = 0;
char a[50];
char pass[50] = " ";
char pwd[50];
int passpos = 0;
int state = 0;
char transilateIR();
int lockstate = 2;
int brightval = -1;
int percentage;

//const int pubinterval = 5000;
long millicount;

//-----( Declare objects )-----
//LCD
LiquidCrystal_I2C lcd ( 0x27 , 2 , 1 , 0 , 4 , 5 , 6 , 7 , 3 , POSITIVE);   //LCD object
//Servo
Servo s;                                 //Servo object
//IRRemote
IRrecv irrecv(receiver);                 //IR receiver object
decode_results results;                 //Object to store decoded data

byte lock[8] = {
  0b00000,
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b11011,
  0b11111
};

byte unlock[8] = {
  0b00000,
  0b01110,
  0b10001,
  0b00001,
  0b00001,
  0b11111,
  0b11011,
  0b11111
};
byte key[8] = {
  0b00110,
  0b00100,
  0b00110,
  0b00100,
  0b00100,
  0b01110,
  0b01010,
  0b01110
};

byte downarrow[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b11111,
  0b01110,
  0b00100
};

void setup()
{
  //Begin all comminications
  lcd.begin(16, 2);
  Serial.begin(9600);
  s.attach(11);
  irrecv.enableIRIn();

  brightval = map(EEPROM.read(100), 0, 255, 0, 9);
  brightness = map(brightval, 0, 9, 0, 255);
  analogWrite(backlightpin, brightness);
  percentage = map(brightness, 0, 255, 0, 100);

  //Position the servo
  s.write(0);

  lcd.clear();
  lcd.home();

  //Set the pin modes
  pinMode(backlightpin, OUTPUT);

  //Create the custom characters of lcd
  lcd.createChar(2, lock);
  lcd.createChar(3, unlock);
  lcd.createChar(4, key);
  lcd.createChar(5, downarrow);

  //Read password from EEPROM and store it to the array pass
  pass[0] = '\0';
  a[0] = '\0';
  lockstate = 2;
  millicount = 0;
  eeprom_read(pwd);
  strcpy(pass, pwd);

  //Print some text on the lcd
  lcd.clear();
  lcd.print("PASS PROTECTED ");
  lcd.setCursor(0, 1);
  lcd.print("  LOCK ");
  lcd.write(2);
  lcd.print(" ");
  lcd.write(3);
  lcd.print(" ");
  lcd.write(4);
  delay(2000);

  Serial.println();
  Serial.println("Pass Protected Lock");

  //Draw the main page
  lcd.setCursor(0, 0);
  drawhome();

}


void loop()
{
  if (lockstate == 2)
  {
    s.write(0);
  }
  else if (lockstate == 3)
  {
    s.write(180);
  }

  char rcv_char = ' ';
  int  input_str_len = 0;

  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    rcv_char = translateIR();
    if (state != 3)
    {
      if (rcv_char == 'L')
      {
        state = 0;
        a[0] = '\0';
        delay(100);
        setup();
      }
    }
    if (rcv_char >= '0' && rcv_char <= '9')
    {
      if (state != 3)
      {
        lcd.setCursor(passpos, 1);
        lcd.print('*');
        passpos = passpos + 1;
        lcd.setCursor(passpos, 1);
      }
      input_str_len = strlen(a);      //get current length of string
      a[input_str_len]   = rcv_char;  //assign received character to the last position in the string
      a[input_str_len + 1] = '\0';    //insert new string terminator

    }

    if (servoRead())
    {
      lockstate = 3;
    }

    else if (!servoRead())
    {
      lockstate = 2;
    }

    //If in its normal state
    if (state == 0)
    {
      if (rcv_char == '-')
      {
        lcd.setCursor(15, 0); lcd.print("!");
        delay(500);
        lcd.setCursor(15, 0); lcd.print(" ");
      }

      if (rcv_char == '!')
      {
        // OK Pressed. Compare password
        Serial.println(rcv_char);
        Serial.println(a);
        if (strcmp(a, pass) == 0)
        {
          lockstate = 3;
          passpos = 0;

          s.write(180);

          lcd.setCursor(0, 1);
          lcd.print("                ");

          lcd.setCursor(0, 1);
          lcd.print("UNLOCKED ");
          lcd.write(3);
          delay(1000);

          lcd.setCursor(0, 1);
          lcd.print("                ");

          drawhome();

          Serial.println("Correct pass");
          if (servoRead())
          {
            lockstate = 3;
          }

          else if (!servoRead())
          {
            Serial.println("The servo is sick");
            lockstate = 2;
          }

        }
        else {
          lockstate = 2;

          passpos = 0;

          s.write(0);


          lcd.setCursor(0, 1);
          lcd.print("WRONG PASS:");
          lcd.print(a);
          delay(300);

          lcd.setCursor(0, 1);
          lcd.print("                ");

          delay(300);

          lcd.setCursor(0, 1);
          lcd.print("WRONG PASS:");
          lcd.print(a);
          delay(300);

          lcd.setCursor(0, 1);
          lcd.print("                ");

          delay(300);

          lcd.setCursor(0, 1);
          lcd.print("WRONG PASS:");
          lcd.print(a);

          delay(1000);

          lcd.setCursor(0, 1);
          lcd.print("                ");

          lcd.setCursor(0, 1);
          lcd.print("LOCKED ");
          lcd.write(2);

          delay(1000);
          lcd.setCursor(0, 1);
          lcd.print("                ");

          drawhome();

          if (servoRead())
          {
            lockstate = 3;
            Serial.println("The servo is sick");
          }

          else if (!servoRead())
          {
            lockstate = 2;
          }

          Serial.println("WRONG PASS");
          lcd.print(a);
        }
        a[0] = '\0';

        lcd.setCursor(0, 1);
        lcd.print("                ");
      }




      if (rcv_char == 'R')
      {
        if (lockstate == 3)
        {
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.print("PLEASE");
          lcd.setCursor(2, 1);
          lcd.print("LOCK IT!  ");
          lcd.setCursor(11, 0);
          lcd.write(5);
          lcd.setCursor(11, 1);
          lcd.write(3);
          delay(400);
          lcd.setCursor(11, 1);
          lcd.write(2);
          delay(1000);
          lcd.clear();
          drawhome();
          state = 0;
          rcv_char = ' ';
        }
        
        else
        {
          state = 1;
          drawst1();
          a[0] = '\0';
          passpos = 0;
        }

      }
      if (rcv_char == 'L')
      {

      }
      if (rcv_char == 'F')
      {
        if (state == 0)
        {
          state = 3;
          drawst3();
          percentage = map(EEPROM.read(100), 0, 255, 0, 100);
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(percentage);
          lcd.print(" %");
          Serial.println(EEPROM.read(100));
        }

      }
      if (rcv_char == 'B')
      {
        setup();
      }
      if (rcv_char == '*')
      {
        a[0] = '\0';
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        passpos = 0;

      }
      if (rcv_char == '#')
      {
        lockstate = 2;
        passpos = 0;
        s.write(0);

        if (servoRead())
        {
          lockstate = 3;
          Serial.println("The servo is sick");
        }

        else if (!servoRead())
        {
          lockstate = 2;
        }

        lcd.setCursor(0, 1);
        lcd.print("                ");

        lcd.setCursor(0, 1);
        lcd.print("LOCKED ");
        lcd.write(2);
        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print("                ");

        drawhome();
      }
    }
    else if (state == 1)
    {

      if (rcv_char == '!')
      {
        if (strcmp(a, pass) == 0)
        {
          state = 2;
          a[0] = '\0';
          drawst2();
          passpos = 0;
        }
        else
        {
          lcd.clear();
          lcd.print("WRONG PASS:");
          lcd.print(a);
          delay(1000);
          state = 0;
          drawhome();
          s.write(0);
          a[0] = '\0';
          passpos = 0;
        }
      }
      else if (rcv_char == '*')
      {
        state = 0;
        drawhome();
        s.write(0);
        a[0] = '\0';
        passpos = 0;
      }
    }

    else if (state == 2)
    {
      if (rcv_char == '!')
      {
        pass[0] = '\0';
        Serial.println(a);
        eeprom_write(a);
        eeprom_read(pwd);
        strcpy(pass, pwd);
        a[0] = '\0';
        lcd.clear();
        s.write(0);
        lcd.print("PASS SAVED");
        Serial.println(pass);
        delay(1000);
        drawhome();
        state = 0;
        passpos = 0;
      }
      else if (rcv_char == '*')
      {
        state = 0;
        drawhome();
        s.write(0);
        a[0] = '\0';
        passpos = 0;
      }
    }

    else if (state == 3)
    {


      if (rcv_char == 'R')
      {
        if (brightval == 9)
        {
          brightval = 0;
          brightness = 0;
          percentage = 0;
          Serial.println(brightval);
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(percentage);
          lcd.print(" %");
          analogWrite(backlightpin, brightness);
          EEPROM.write(100, brightness);
          Serial.println(brightness);
        }
        else if (brightval < 9)
        {
          ++brightval;
          brightness = map(brightval, 0, 9, 0, 255);
          percentage = map(brightness, 0, 255, 0, 100);
          Serial.println(brightval);
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(percentage);
          lcd.print(" %");
          analogWrite(backlightpin, brightness);
          EEPROM.write(100, brightness);
          Serial.println(brightness);
        }
      }
      else if (rcv_char == 'L')
      {
        if (brightval == 0)
        {
          brightval = 9;
          brightness = 255;
          percentage = 100;
          Serial.println(brightval);
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(percentage);
          lcd.print(" %");
          analogWrite(backlightpin, brightness);
          EEPROM.write(100, brightness);
          Serial.println(brightness);
        }
        else if (brightval > 0)
        {
          --brightval;
          brightness = map(brightval, 0, 9, 0, 255);
          percentage = map(brightness, 0, 255, 0, 100);
          Serial.println(brightval);
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(percentage);
          lcd.print(" %");
          analogWrite(backlightpin, brightness);
          EEPROM.write(100, brightness);
          Serial.println(brightness);
        }
      }
      else if (rcv_char == 'F')
      {
        analogWrite(backlightpin, brightness);
        a[0] = '\0';
        state = 0;
        drawhome();
      }



    }


    delay(500);
    irrecv.resume(); // receive the next value
  }

}

char translateIR() // Takes action based on IR code received
{
  int i = 0;
  char rcv_char = ' ';
  switch (results.value)

  {

    case 0x511DBB: Serial.println(" FORWARD"); rcv_char = 'F'; break;
    case 0x52A3D41F: Serial.println(" LEFT");  rcv_char = 'L'; break;
    case 0xD7E84B1B: Serial.println(" -OK-");  rcv_char = '!'; break;
    case 0x20FE4DBB: Serial.println(" RIGHT"); rcv_char = 'R'; break;
    case 0xA3C8EDDB: Serial.println(" REVERSE"); rcv_char = 'B'; break;
    case 0xC101E57B: Serial.println(" 1");  rcv_char = '1'; break;
    case 0x97483BFB: Serial.println(" 2");  rcv_char = '2'; break;
    case 0xF0C41643: Serial.println(" 3");  rcv_char = '3'; break;
    case 0x9716BE3F: Serial.println(" 4");  rcv_char = '4'; break;
    case 0x3D9AE3F7: Serial.println(" 5");  rcv_char = '5'; break;
    case 0x6182021B: Serial.println(" 6");  rcv_char = '6'; break;
    case 0x8C22657B: Serial.println(" 7");  rcv_char = '7'; break;
    case 0x488F3CBB: Serial.println(" 8");  rcv_char = '8'; break;
    case 0x449E79F: Serial.println(" 9");   rcv_char = '9'; break;
    case 0x32C6FDF7: Serial.println(" *");  rcv_char = '*'; break;
    case 0x1BC0157B: Serial.println(" 0");  rcv_char = '0'; break;
    case 0x3EC3FC1B: Serial.println(" #");  rcv_char = '#'; break;
    case 0xFFFFFFFF: Serial.println(" REPEAT"); rcv_char = '-'; break;

    default:
      lcd.setCursor(15, 0); lcd.print("!");
      delay(500);
      lcd.setCursor(15, 0); lcd.print(" ");
      Serial.println(" other button   "); Serial.println(results.value, HEX);

  }// End Case

  return rcv_char;
}

void eeprom_write(char *pwd)
{
  char b[50] = "";
  //        Serial.println(pwd);
  int pwdlen =  strlen(pwd);
  int i;
  for (i = 0; i < pwdlen; i++)
  {
    EEPROM.write(i, pwd[i]);
  }
  EEPROM.write(i, '\0');
  //  EEPROM.put(0, pwd);
  EEPROM.get(0,  b);
  Serial.println(b);

}

void eeprom_read(char * pwd)
{
  uint8_t c;
  int i = 0;
  c = EEPROM.read(0);
  while (c > 0)
  {
    pwd[i] = c;
    i++;
    c = EEPROM.read(i);
  }
  pwd[i] = '\0';
  //  EEPROM.get(0, pwd);
}

void drawst1()
{
  lcd.clear();
  lcd.print("ENTER OLD PASS:");
  lcd.setCursor(0, 1);
  passpos = 0;
}

void drawst2()
{
  lcd.clear();
  lcd.print("ENTER NEW PASS:");
  lcd.setCursor(0, 1);
  passpos = 0;
}

void drawst3()
{
  lcd.clear();
  lcd.print("SET BRIGHTNESS");
  lcd.setCursor(5, 1);
}
void drawhome()
{

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER PASS: ");
  lcd.write(lockstate);
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  a[0] = '\0';
}

bool servoRead()
{
  int d = s.read();

  if (d == 180)
  {
    lockstate = 3;
    return true;
  }
  else if (d == 0)
  {
    lockstate = 2;
    return false;
  }

}
