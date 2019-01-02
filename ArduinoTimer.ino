#include <LiquidCrystal.h>
#include <Keypad.h>
#include <stdlib.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Pins Used for LCD Display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // LCD Display initialized with given pins

const int lcdWidth = 16;
const int lcdHeight = 2;

const byte columnNumber = 4; // Number of Columns on Keypad
const byte rowNumber    = 4; // Number of Rows on Keypad

char keys[rowNumber][columnNumber] = {  //=============
  {'1','2','3', 'A'},                   //
  {'4','5','6', 'B'},                   //  Keypad Keys Defined in Rows and Columns
  {'7','8','9', 'C'},                   //    
  {'#','0','*', 'D'}                    //
};                                      //=============     

byte rowPins[rowNumber] = {A1, A2, A3, A4}; // Keypad Columns Connection
byte columnPins[columnNumber] = {9, 8, 7, 6}; // Keypad Rows Connection

Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, columnPins, rowNumber, columnNumber); // Construct Keypad with given parameters 

char minutes[2];
char seconds[2];
int minute = 0, second = 0;
int index = 0;
bool timerRunning;

void setup()
{
    lcd.begin(lcdWidth, lcdHeight);
    Serial.begin(9600);
}

void loop()
{
    inputTime();
    Serial.println(minutes);
}

void inputTime()
{
    char c = myKeypad.getKey();
    if(c)
    {
        switch(index)
        {
            case 0: minutes[0] = c; break;
            case 1: minutes[1] = c; break;
            case 2: seconds[0] = c; break;
            case 3: seconds[1] = c; break;
        }
        index++;
        if(index == 4)
        {
            index = 0;
            lcd.setCursor(index, 0);
            minute = atoi(minutes);
            second = atoi(seconds);
        }
    }  
}
