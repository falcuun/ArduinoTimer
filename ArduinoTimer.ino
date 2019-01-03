#include <LiquidCrystal.h>
#include <Keypad.h>
#include <stdlib.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Pins Used for LCD Display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                  // LCD Display initialized with given pins

const int lcdWidth = 16;
const int lcdHeight = 2;

const byte columnNumber = 4; // Number of Columns on Keypad
const byte rowNumber = 4;    // Number of Rows on Keypad

//===============================================================
char keys[rowNumber][columnNumber] = {

    {'1', '2', '3', 'A'}, //
    {'4', '5', '6', 'B'}, //  Keypad Keys Defined in Rows and Columns
    {'7', '8', '9', 'C'}, //
    {'#', '0', '*', 'D'}  //
};
//===============================================================

byte rowPins[rowNumber] = {9, 8, 7, 6};           // Keypad Columns Connection
byte columnPins[columnNumber] = {A1, A2, A3, A4}; // Keypad Rows Connection

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowNumber, columnNumber); // Construct Keypad with given parameters

const char timer[] = {'T', 'i', 'm', 'e', 'r', ':', ' ', '\0'}; // Charr Array for holding "Timer: " text
const int timer_length = sizeof(timer) - 1;                     // Length of Timer Array (text) - 1 for beginning cursor position on LCD screen
const int MAX_INDEX = timer_length + 5;                         // Variable storing max value of Index
char minutes[3], seconds[3];                                    // Char Arrays to be turned into an int
int minute = 0, second = 0, index = timer_length;               // Variables holding values from char arrays
bool timerRunning = true;                                       // State of the Timer, Running/Not Running

void setup()
{
    lcd.begin(lcdWidth, lcdHeight); // Initializing the interface to the LCD screen with given size (lcdW, lcdH)
    lcd.print(timer);               //
    lcd.print("00:00");             // Initial Print on the Screen printing "Timer: 00:00"
    lcd.setCursor(timer_length, 0); // Setting cursor of LCD to the first space after 0 after "Timer: 00:00"
    Serial.begin(9600);             // Initializing Serial communication with 9600 baud rate
}

void loop()
{
    inputTime(); // Calling the inpuTime() method
}

void inputTime()
{
    lcd.blink();                // Start blinking the cursor on the screen when the method is called
    char c = myKeypad.getKey(); // Get a key value from the keypad and parse it into a character
    if (c)
    {
        switch (index)
        {
        case timer_length:
            minutes[0] = c;          // Assign current c character as a first element of the Minutes[] array
            lcd.setCursor(index, 0); // Move the cursor of the LCD to index place
            lcd.print(c);            // Print out the input Character to the LCD
            break;
        case (timer_length + 1):
            minutes[1] = c;          // Assign current c character as second element of the Minutes[] array
            minutes[2] = '\0';       // Adding '\0' character to mark the end of the string
            lcd.setCursor(index, 0); // Move the cursor of the LCD to index place
            lcd.print(c);            // Print out the input Character to the LCD
            index++;                 // Increment the index value
            lcd.setCursor(index, 0); // Move the cursor of the LCD to Index place
            lcd.print(':');          // Print ':' as a divider for MM:SS format
            break;
        case (timer_length + 3):
            seconds[0] = c;
            lcd.setCursor(index, 0);
            lcd.print(c);
            break;
        case (timer_length + 4):
            seconds[1] = c;
            seconds[2] = '\0';
            lcd.setCursor(index, 0);
            lcd.print(c);
            break;
        }
        index++;
        if (index == MAX_INDEX)
        {
            index = timer_length;    // Reset the Index value to the beginning of the timer
            lcd.setCursor(index, 0); // Move the cursor of the LCD back to beginning of timer
            minute = atoi(minutes);  // Convert the Minutes Charr array to integer
            second = atoi(seconds);  // Convert the Seconds Charr array to integer
            lcd.noBlink();           // Stop the blinking of the LCD Cursor
            runTimer();              // Call runTimer() method
        }
    }
}

void runTimer()
{
    while (timerRunning)
    {
        lcd.clear();  // Clear the LCD Screen
        printToLCD(); // Call printToLCD() method
        if (second <= 0)
        {
            if (minute <= 0)
            {
                timerRunning = false; // If bothe Minutes and Seconds are at 0 (Timer is over) set timer state to FALSE (Not Running)
                lcd.clear();          // Clear the LCD Screen
                lcd.print("END");     // Print "END" on the Screen
            }
            else
            {
                minute--;    // Decrement the minutes for one
                second = 59; // Set the seconds to 59 for next minute
            }
        }
        else
        {
            second--; // Decrement the seconds for one
        }
        delay(1000); // Wait for one second
    }
}

void printToLCD()
{
    lcd.print(timer); // Print "Timer: " to the LCD Screen
    if (minute < 10)
    {
        lcd.print('0'); // If Minutes are less than 10 print out a '0' before printing minutes value for the MM format (ex. 01:00)
    }
    lcd.print(minute); // Print the minutes value
    lcd.print(':'); // Print the divider of Minutes and Seconds
    if (second < 10)
    {
        lcd.print('0'); // If Seconds are less than 10 print out a '0' before printing seconds value for the SS format (ex. 01:05)
    }
    lcd.print(second); // Print the seconds value
}

/*

    TODO 1: Add Array Of All the Numbers and Letters from Keyboard
    TODO 2: Add a Way to Check for the Buttons and Perform an action accordingly. 
    TODO 3: Add a Way to Start Timer on * Button
    TODO 4: Add a Way to Stop Timer on # Button

*/