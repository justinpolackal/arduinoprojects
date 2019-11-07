
#include <EEPROM.h> //for storing highscores

#include <LiquidCrystal.h> //lcd library
#include <LCDKeypad.h> //interface with lcd keypad shield

LCDKeypad lcd; //create lcd object

//variables required
int addr = 0;
int key = 0;
int button = 16;
int walk = 1;
int pitch = 800;
int block;
int fieldArray[25][2];
int lastBlockPos;
int lastBlockPosTop;
int crash = 0;
int scrolls = 0;
int score = 0;
int Delay = 300;
int highscore = 0;
char* scorestate;

//define functions
void initialize_field_array(int x, int y, int charnum);
void display_field_array();
//all the symbols required
byte walkingman[8] = {

  0b01100,
  0b01100,
  0b00000,
  0b01110,
  0b11100,
  0b01100,
  0b11010,
  0b10011
};
byte jumpman[8] = {
  0b01100,
  0b01100,
  0b00000,
  0b11110,
  0b01101,
  0b11111,
  0b10000,
  0b00000
};
byte walkingman2[8] = {
  B01100,
  B01100,
  B00000,
  B01100,
  B01100,
  B01100,
  B01100,
  B01110,
};
byte Block[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte Clear[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000

};
byte arrow[8] = {
  0b11000,
  0b11100,
  0b11110,
  0b11111,
  0b11111,
  0b11110,
  0b11100,
  0b11000,
  
};

/*
   Intialize field array to all blanks.
*/
void initialize_field_array(int x, int y, int charnum)
{
  int i, j;
  //loop through every positions
  for (i = 0; i < 21; i++)
  {
    for (j = 0; j < 2; j++)
    {
      //delay(500);
      fieldArray[i][j] = 3; //clear the current position
    }

  }
  fieldArray[x][y] = charnum; //add character
  fieldArray[15][1] = 2; //The first block
  fieldArray[18][0] = 2;
  lastBlockPos = 15; //The latest block is set at position 15
  lastBlockPosTop = 18;
  //delay(500);
  return ;
}

void display_field_array() //this function prints the field array on lcd display
{
  int i, j; //define two variables for x and y loops
  for (i = 0; i < 21; i++)
  {
    for (j = 0; j < 2; j++)
    {
      lcd.setCursor(i, j); //Set cursor to selected location
      lcd.write(fieldArray[i][j]); //print data
    }
  }
  return ;
}


void setup()
{
  //setup
  key = 0;
  lcd.begin(16, 2);
  lcd.home(); //set cursor to 0,0
  lcd.print (" Arduino game");
  // delay(100);
  lcd.setCursor(0, 1);
  lcd.print("  by:NotesPoint");
  // delay(1000);
  //intro
  int delayc = 1;
  for (int positionCounter = 0; positionCounter < 13; positionCounter++)
  {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    delayc += 3;
    delay(delayc);
  }
  for (int positionCounter = 0; positionCounter < 13; positionCounter++)
  {
    // scroll one position right:
    lcd.scrollDisplayRight();
    delayc += 3;
    delay(delayc);
  }
  // delay(500);
  //delay(150);
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayRight();
    delayc += 3;
    delay(delayc);
  }
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayLeft();
    delayc += 3;
    delay(delayc);
  }
  int i = 0;
  for (i = 0; i < 16; i++)
  {
    lcd.setCursor(i,0);
    lcd.print(" ");
    delay(7);
  }
  for (i = 0; i < 16; i++)
  {
    lcd.setCursor(i,1);
    lcd.print(" ");
    delay(7);
  }
  delay(1000);
  lcd.clear();
  lcd.print("highscore:");
  lcd.write(7);
  lcd.print(" ");
  lcd.print(EEPROM.read(addr)); //read the highscore from eeprom and print
  delay(1000);
  if (EEPROM.read(addr) != 0) //if any highscore is saved,
    highscore = EEPROM.read(addr); //set it to highscore variable
  //delay(1000);
  lcd.clear();
  //create the necessary characters
  lcd.createChar(1, walkingman);
  lcd.createChar(2, Block);
  lcd.createChar(3, Clear);
  //lcd.createChar(4, explode);
  lcd.createChar(5, jumpman);
  lcd.createChar(6, walkingman2);
  lcd.createChar(7, arrow);

  lcd.clear();
  lcd.home();

  lcd.print("Press to start!"); 
  while (lcd.button() != KEYPAD_SELECT) {
    lcd.setCursor(0, 1);
    lcd.write(1);
    delay(100);
    lcd.setCursor(0, 1);
    lcd.write(6);
    delay(100);// lcd.home();
    senseReset();
  }
  lcd.noAutoscroll();
  lcd.clear();

  initialize_field_array(3, 1, 1); //clear field array
  display_field_array(); //print field array on lcd
  Serial.begin(9600); //start serial communication
 
}
void loop() {
  key = 0;
//  if (walk == 1) {
//    walk = 6;
//  }
//  else if (walk == 6) {
//    walk = 1;
//  }
  // Serial.println(walk);
  if (EEPROM.read(addr) != 0) { //read current highscore if any
    highscore = EEPROM.read(addr);
  }
  senseReset(); //if right key is pressed, reset highscore and start from the setup() function
  if (crash == 1) //if crashed on any block
  {
    delay(500); //wait a bit
    //if crossed the previous highscore, write the new one to eeprom
    if (highscore == 0) {
      highscore = score;
      scorestate = "highscore:";
      EEPROM.put(addr, highscore);
    }
    else if (highscore > score) {
      scorestate = "score: ";
    }
    else if (score > highscore) {
      scorestate = "highscore: ";
      highscore = score;
      EEPROM.put(addr, highscore);
    }
    //tell us that you have crashed
    lcd.clear();
    lcd.setCursor(10, 0);
    lcd.print("crash!");
    lcd.setCursor(0, 1);
    lcd.print(scorestate);
    //lcd.print(":");
    lcd.print(score);
    delay(1500);

    crash = 0; //set crash to 0
    displaycrash(); //
  }

  display_field_array(); //print field array to lcd
  insert_block(2); //randomly insert obstacles in the way
  key = getinput(); //read keypad and if select key pressed, make the character jump
  if (key == KEYPAD_SELECT) //if select key pressed
    display_field_array(); //print field array to lcd
  scroll_left(3, 1, 1, 5); //make everything move
  if (fieldArray[3][0] == 5 ) //if the character is up in the air
  {
    countscroll(); //start counting air time
    if (scrolls >= 4 && fieldArray[3][1] != 2) //if scrolled 4 times and there is no obstacle below, charcter reaches the ground
    {
      jumpdown(); //
      scrolls = 0;
    }
    // jumpdown();
    display_field_array(); //print field array to lcd
  }

  countscore(); //count the score based on the number of obstacles dodged
  lcd.setCursor(0, 0);
  lcd.print(score);
  key = 0;
  sensePause(); //check is the left key is pressed i.e pause key
  display_field_array(); //print field array to lcd
  delay(100); //wait a bit before the next loop


}
void senseReset()
{
  if (lcd.button() == KEYPAD_RIGHT) { //if clicked right key
    EEPROM.put(addr, 0); //reset highscore
    highscore = EEPROM.read(addr); 
    setup(); //start from the begenning
  }
}
void sensePause()
{
  if (lcd.button() == KEYPAD_LEFT) //if left key pressed, pause
  { 
    lcd.clear();
    lcd.print("Paused");
    while (lcd.button() == KEYPAD_LEFT) //wait until left key is released
    {
      lcd.setCursor(0, 1);
      lcd.write(1);
      delay(100);
      lcd.setCursor(0, 1);
      lcd.write(6);
      delay(100);// lcd.home();
      senseReset();
    }
    while (lcd.button() != KEYPAD_LEFT) //wait until left key is pressed
    {
      lcd.setCursor(0, 1);
      lcd.write(1);
      delay(100);
      lcd.setCursor(0, 1);
      lcd.write(6);
      delay(100);// lcd.home();
      senseReset();
    }
    while (lcd.button() == KEYPAD_LEFT) //wait until left key is released to avoid false triggers
    {
      lcd.setCursor(0, 1);
      lcd.write(1);
      delay(100);
      lcd.setCursor(0, 1);
      lcd.write(6);
      delay(100);// lcd.home();
      senseReset();
    }
    display_field_array(); 
    //say 3,2,1 go!
    lcd.setCursor(0, 0);
    lcd.print("3");
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("2");
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("1");
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("GO!");
    delay(1000);
  }
}
void scroll_left(int x, int y, int charnum, int jump) //move everything, scroll everything to the left
{
  int i, j;
  j = 1; //second line of the lcd first
  for (i = 0; i < 21; i++) //loop through field array
  {
    //scrolls + 1;
    // Serial.print(scrolls);
    if (fieldArray[i][j] == charnum) //if current position is the position of the character,
    {
      if (fieldArray[i + 1][j] == 2) //if the next position contains an obstacle
      {
        crash = 1; //crash!
        fieldArray[i][j] = 2;
      }
      else
      {
        crash = 0; //if the next position isn't an obstacle, don't crash
      }
    }
    else //if current pos is not the character
    {
      if (fieldArray[i + 1][j] != charnum )
      {
        fieldArray[i][j] = fieldArray[i + 1][j]; //scroll current pos to the left
      }
      else //if this is not there, when character lands after jumping and if the previous pos is a block, something funny will happen
      {
        //        if(fieldArray[i][j] == 2)
        //        {
        fieldArray[i][j] = 3;
        //        }
      }
    }
    if (fieldArray[i][j] == 2) //set the latest position of the block
    {
      lastBlockPos = i;
    }
  }
  //clear the last pos of field array
  fieldArray[i][0] = 3;
  fieldArray[i][1] = 3;

  //time to scroll the top row of lcd
  j = 0;
  //most of it is pretty much the same
  for (i = 0; i < 21; i++) //loop through field array
  {
    if (fieldArray[i][j] == jump) //since it is the top row, check for the jumping character
    {
      if (fieldArray[i + 1][j] == 2) //if the next pos contains an obstacle
      {
        crash = 1; //crash!
        fieldArray[i][j] = 2;
      }
      else
      {
        crash = 0; //if the next position isn't an obstacle, don't crash
      }
    }
    else //if current pos is not the character
    {
      if (fieldArray[i + 1][j] != jump)
      {
        fieldArray[i][j] = fieldArray[i + 1][j]; //scroll current pos to the left
      }
      else //if this is not there, when character lands after jumping and if the previous pos is a block, something funny will happen
      {
        //        if(fieldArray[i][j] == 2)
        //        {
        fieldArray[i][j] = 3;
        //        }
      }
    }
    if (fieldArray[i][j] == 2) //set the latest position of the block
    {
      lastBlockPosTop = i;
    }
  }
  //fieldArray[i][0] = 3;
  //fieldArray[i][1] = 3;

}

void insert_block(int charnum) //this function randomly inserts blocks
{
  //create two variables
  int rand_number; 
  int randnumber;
  int i = 0;
  i = 15;
  if ( (15 - lastBlockPos) > 2 && (15 - lastBlockPosTop) > 2) //if there is enough space to create a block - don't create a block on top just after creating one down
  {
    //create two random numbers
    randnumber = random(1, 2);
    rand_number = random(1, 2);
    if (randnumber == rand_number) //if both random numbers are the same, insert a block
    { 
      //fieldArray[15][1] = charnum;
      if (random(1, 3) == random(1, 3) ) //if two random numbers are equal,
      {
        for (i = 16; i < random(16, 20); i++) //create blocks on the second row of lcd
        {
          fieldArray[i][1] = charnum;
        }
      }
      else //if the two random numbers are not equal,
      {
        for (i = 18; i < random(18, 21); i++) //create blocks on the top row
        {
          fieldArray[i][0] = charnum;
        }
      }
    }


  }
}

int getinput() //check for user input
{
  //tone(14, 6000, 5);//delay(1)
  key = 0;
  key = lcd.button();
  if (key == KEYPAD_SELECT) {
    //delay(10);
    if (fieldArray[3][1] != 2)
    {
      fieldArray[3][1] = 3;
    }
    fieldArray[3][0] = 5;


  }
  return key;
  key = 0;
}

void displaycrash() //display the menu when crashed
{
  score = 0;
  scrolls = 0;
  display_field_array();
  fieldArray[3][1] = 3;
  lcd.home();
  lcd.print("Press to start!");
  while (lcd.button() != KEYPAD_SELECT) {
    lcd.setCursor(0, 1);
    lcd.write(1);
    delay(100);
    lcd.setCursor(0, 1);
    lcd.write(6);
    delay(100);// lcd.home();
    senseReset();
  }
  lcd.clear(); //clear the screen
  initialize_field_array(3, 1, 1);
  display_field_array();
  // delay();
  key = 0;
  fieldArray[3][1] = 1;
  scrolls = 0;
  score = 0;
  key = 0;

}

void jumpdown() //land after jumping
{
  // Serial.print("jumpdown");
  // if(fieldArray[5][1] == 2)
  //  {
  key = 0;
  fieldArray[3][0] = 3; //clear the character from top row
  fieldArray[3][1] = 1; //draw it in the bottom row

  // }
}

void countscroll() //count the jump distance
{
  //key = 0;
  scrolls = scrolls + 1;
  key = 0;
}

void countscore() //keep increasing the score when dodging blocks
{
  if (fieldArray[1][1] == 2)
    score = score + 1;
}

