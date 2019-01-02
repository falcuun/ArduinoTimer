#include <LiquidCrystal.h>
#include <Keypad.h>
#include <stdlib.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Pins Used for LCD Display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                  // LCD Display initialized with given pins

const int lcdWidth = 16;
const int lcdHeight = 2;

const byte columnNumber = 4; // Number of Columns on Keypad
const byte rowNumber = 4;    // Number of Rows on Keypad

char keys[rowNumber][columnNumber] = {
                          //=============
    {'1', '2', '3', 'A'}, //
    {'4', '5', '6', 'B'}, //  Keypad Keys Defined in Rows and Columns
    {'7', '8', '9', 'C'}, //
    {'#', '0', '*', 'D'}  //
};                        //=============

byte rowPins[rowNumber] = {9, 8, 7, 6};           // Keypad Columns Connection
byte columnPins[columnNumber] = {A1, A2, A3, A4}; // Keypad Rows Connection

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowNumber, columnNumber); // Construct Keypad with given parameters

char minutes[3], seconds[3];           // Char Arrays to be turned into an int
int minute = 0, second = 0, index = 0; // Variables holding values from char arrays
bool timerRunning = true;              // State of the Timer, Running/Not Running

void setup()
{
    lcd.begin(lcdWidth, lcdHeight);
    Serial.begin(9600);
}

void loop()
{
    inputTime();
}

void inputTime()
{
    lcd.blink();
    char c = myKeypad.getKey();
    if (c)
    {
        switch (index)
        {
        case 0:
            minutes[0] = c;
            lcd.setCursor(index, 0);
            break;
        case 1:
            minutes[1] = c;
            minutes[2] = '\0';
            lcd.setCursor(index, 0);
            break;
        case 2:
            seconds[0] = c;
            lcd.setCursor(index, 0);
            break;
        case 3:
            seconds[1] = c;
            seconds[2] = '\0';
            lcd.setCursor(index, 0);
            break;
        }
        index++;
        if (index == 4)
        {
            index = 0;
            lcd.setCursor(index, 0);
            minute = atoi(minutes);
            second = atoi(seconds);
            runTimer();
        }
    }
}

void runTimer()
{
    while (timerRunning)
    {
      lcd.clear();
        if (second <= 0)
        {
            if (minute <= 0)
            {
                timerRunning = false;
            }
            else
            {
                minute--;
                second = 59;
            }
        }
        else
        {
            second--;
        }
        printToLCD();
        delay(1000);
    }
}

void printToLCD()
{
    if (minute < 10)
    {
        lcd.print('0');
    }
    lcd.print(minute);
    lcd.print(':');
    if (second < 10)
    {
        lcd.print('0');
    }
    lcd.print(second);
}