#include <LiquidCrystal.h>
#include <Keypad.h>
#include <stdlib.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Pins Used for LCD Display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                  // LCD Display initialized with given pins

const int lcdWidth = 16; // Width of the LCD Screen
const int lcdHeight = 2; // Height of the LCD Screen

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

/*
 * Possible Input Values
*/
const char keypad_letters[] = {'A', 'B', 'C', 'D'};                                 // Letter Characters Input
const char special_characters[] = {'*', '#'};                                       // Special Characters Input
const char numerical_keypad[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}; // Numerical Keypad Inputs

/*
 * Constants used to controll the logic 
*/
const char timer[] = {'T', 'i', 'm', 'e', 'r', ':', ' ', '\0'}; // Charr Array for holding "Timer: " text
const int timer_length = sizeof(timer) - 1;                     // Length of Timer Array (text) - 1 for beginning cursor position on LCD screen
const int MAX_INDEX = timer_length + 6;                         // Variable storing max value of Index

/*
  * Main variables that logic works with.
 */
char minutes[3], seconds[3];                      // Char Arrays to be turned into an int
int minute = 0, second = 0, index = timer_length; // Variables holding values from char arrays
bool timerRunning = false;                        // State of the Timer, Running/Not Running

/*
 * Setup of the Arduino and it's peripherals 
*/
void setup()
{
    lcd.begin(lcdWidth, lcdHeight); // Initializing the interface to the LCD screen with given size (lcdW, lcdH)
    lcd.print(timer);               //
    lcd.print("00:00");             // Initial Print on the Screen printing "Timer: 00:00"
    lcd.setCursor(timer_length, 0); // Setting cursor of LCD to the first space after 0 after "Timer: 00:00"
    Serial.begin(9600);             // Initializing Serial communication with 9600 baud rate
}

/*
 * Main loop of the arduino
*/
void loop()
{
    if (inputTime())
    {
        char c = myKeypad.getKey(); // Get a key value from the keypad and parse it into a character
        checkInput(c);
    }
}

/*
 * This method will check if the input character mathes any character from the input arrays declared above. 
 * Current setup only checks for Special Characters in the array of special Characters
 * But it can serve as an example of the logic used to check the input
*/
void checkInput(char c)
{
    if (c)
    {

        for (int i = 0; i < sizeof(special_characters); i++)
        {
            if (c == special_characters[i])
            {
                specialCharacters(c);
            }
        }
    }
}

// Unused // Only for 4x4 Keypads
/*
 * This method checks if the input is any of the Letter Characters provided you have a 4x4 keypad for your Arduino
 * No special action is taking place on click of any of those due to the fact that they are unused.
*/
void letterCharacters(char c)
{
    switch (c)
    {
    case 'A':
        lcd.print(c);
        break;
    case 'B':
        lcd.print(c);
        break;
    case 'C':
        lcd.print(c);
        break;
    case 'D':
        lcd.print(c);
        break;
    }
}
/*
 * This method parses the special Character input (*, #) 
 * If the input matches one of the cases, this method will call for other methods to execute
 * Thus either starting the timer (#) or clearing the input (*)
*/
void specialCharacters(char c)
{
    if (c)
    {
        switch (c)
        {
        case '*':
            timerRunning = false;
            clearInput();
            break;
        case '#':
            timerRunning = true;
            runTimer();
            break;
        }
    }
}

/*
*   This Method returns true only when there is all numbers inputed to the logic.
*   Method works by creating it's own character that will be inserted in the variables.
*   Position of the insertion is hardcoded because this method serves only one purpose.
*/
bool inputTime()
{
    char c = myKeypad.getKey();
    if (c)
    {
        lcd.blink(); // Start blinking the cursor on the screen when the method is called
        lcd.setCursor(index, 0);
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
            case (timer_length + 5):
                specialCharacters(c);
                lcd.setCursor(index, 0);
                lcd.print(c);
                return true;
                break;
            }
            index++;
            return false;
        }
    }
}

/*
 * This method can only get called AFTER the input is called, you can perform a check 
 * before running the logic to see if the input returned TRUE or FALSE so there wouldn't 
 * be any errors.
 * Method takes in the values from char array variables and calls stdlib's atoi method to 
 * stitch those into an int that will be used for timer.
 */
void assignNumbers()
{
    index = timer_length;    // Reset the Index value to the beginning of the timer
    lcd.setCursor(index, 0); // Move the cursor of the LCD back to beginning of timer
    minute = atoi(minutes);  // Convert the Minutes Charr array to integer
    second = atoi(seconds);  // Convert the Seconds Charr array to integer
    if (minute >= 60)
    {
        minute = 59; // If minutes are greater than or equal to 60, automatically set it to max minute value (59)
    }
    if (second >= 60)
    {
        second = 59; // If seconds are greater than or equal to 60, automatically set it to max second value (59)
    }
    lcd.noBlink(); // Stop the blinking of the LCD Cursor
    timerRunning = true;
}

/*
 * This method clears the text on the screen, prints back the default message (Timer: 00:00)
 * Sets the Cursor back to the first 0, starts the blinking of the cursor and calls 
 * inputTime method again, for repeated action and getting a new number.
 * After that, it calls assignNumbers method
*/
void clearInput()
{
    lcd.clear(); // Clears the Screen of any text
    lcd.print(timer);
    lcd.print("00:00");
    lcd.setCursor(index, 0);
    lcd.blink();
    if (inputTime())
    {
        assignNumbers();
    }
}

/*
 * This method assigns all the numbers to their respected variables and starts a while loop
 * The loop will execute for as long as the timerRunning doesn't equal FALSE
 * Which will happen once the timer hits 00:00 when the message on the display will say "END"
 * Timer counts down by using Arduino's delay(ms) method. It wats for 1000 ms (Or one second)
 * and after each itteration through the loop (After each second) it decrements seconds and minutes 
 * by the given condition
*/
void runTimer()
{
    assignNumbers();
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

/*
 * This Method is doing the printing logic. It sends all the needed data to the LCD
*/
void printToLCD()
{
    lcd.print(timer); // Print "Timer: " to the LCD Screen
    if (minute < 10)
    {
        lcd.print('0'); // If Minutes are less than 10 print out a '0' before printing minutes value for the MM format (ex. 01:00)
    }
    lcd.print(minute); // Print the minutes value
    lcd.print(':');    // Print the divider of Minutes and Seconds
    if (second < 10)
    {
        lcd.print('0'); // If Seconds are less than 10 print out a '0' before printing seconds value for the SS format (ex. 01:05)
    }
    lcd.print(second); // Print the seconds value
}