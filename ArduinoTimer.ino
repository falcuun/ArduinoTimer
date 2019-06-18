#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <stdlib.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[4] = {2, A0, 4, A1};
byte colPins[4] = {A2, 7, 8, 9};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int cursor_position = 0;
int minutes_int;
int seconds_int;
String minutes_string = "";
String seconds_string = "";
bool all_numbers = false;
bool timerRunning = false;
const long interval = 1000;
unsigned long previousMillis = 0;

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.begin(16, 2);
    lcd.blink();
    lcd.setCursor(0, 0);
    Serial.begin(9600);
}

void loop()
{
    char customKey = customKeypad.getKey();
    if (!all_numbers)
    {
        print_numbers(customKey);
    }
    else
    {
        take_special(customKey);
    }
}

void stop_timer()
{
    previousMillis = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Timer Stopped");

    all_numbers = false;
    timerRunning = false;
    digitalWrite(A3, LOW);
    tone(3, 1500, 1000);
    delay(500);
    lcd.clear();
    reset_values();
}

void reset_values()
{
    cursor_position = 0;
    minutes_int = 0;
    seconds_int = 0;
    minutes_string = "";
    seconds_string = "";
}

void start_timer()
{
    tone(3, 2500, 300);
    digitalWrite(A3, HIGH);
    while (timerRunning)
    {
        unsigned long currentMillis = millis();
        char customKey = customKeypad.getKey();
        if (customKey)
        {
            if (customKey == '#')
            {
                stop_timer();
                break;
            }
        }
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;
            lcd.clear();
            if (seconds_int <= 0)
            {
                if (minutes_int <= 0)
                {
                    stop_timer();
                    lcd.clear();
                    tone(3, 1500, 1000);
                    delay(500);
                }
                else
                {
                    if (minutes_int < 10)
                    {
                        lcd.print('0');
                    }
                    lcd.print(minutes_int);
                    lcd.print(':');
                    if (seconds_int < 10)
                    {
                        lcd.print('0');
                    }
                    lcd.print(seconds_int);
                    minutes_int--;
                    seconds_int = 59;
                }
            }
            else
            {
                if (minutes_int < 10)
                {
                    lcd.print('0');
                }
                lcd.print(minutes_int);
                lcd.print(':');
                if (seconds_int < 10)
                {
                    lcd.print('0');
                }
                lcd.print(seconds_int);
                seconds_int--;
            }
        }
    }
}

void take_special(char customKey)
{
    if (customKey)
    {
        switch (customKey)
        {
        case '*':
            timerRunning = true;
            start_timer();
            break;
        case '#':
            timerRunning = false;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
            break;
        }
    }
}

void print_numbers(char customKey)
{
    if (customKey)
    {
        if (customKey != '*' ||
            customKey != '#' ||
            customKey != 'A' ||
            customKey != 'B' ||
            customKey != 'C' ||
            customKey != 'D')
        {
            switch (cursor_position)
            {
            case 0:
                assign_minutes(customKey);
                lcd.print(customKey);
                cursor_position++;
                break;
            case 1:
                assign_minutes(customKey);
                lcd.print(customKey);
                cursor_position++;
                lcd.print(":");
                cursor_position++;
                break;
            case 3:
                assign_seconds(customKey);
                lcd.print(customKey);
                cursor_position++;
                break;
            case 4:
                assign_seconds(customKey);
                lcd.print(customKey);
                cursor_position++;
                check_input(minutes_string, seconds_string);
                all_numbers = true;
                break;
            }
        }
    }
}

void check_input(String minutes, String seconds)
{
    if (minutes_int > 60)
    {
        minutes_int = 60;
    }
    if (seconds_int > 60)
    {
        seconds_int = 60;
    }
}

int assign_minutes(char customKey)
{
    minutes_string += customKey;
    minutes_int = minutes_string.toInt();
    return minutes_int;
}

int assign_seconds(char customKey)
{
    seconds_string += customKey;
    seconds_int = seconds_string.toInt();
    return seconds_int;
}
