/*
Basic keypad (with lib)
*/

// Include section
#include <Keypad.h>

// Constant section
const byte NR_ROWS = 4;
const byte NR_COLS = 4;

char keys[NR_ROWS][NR_COLS] = { // array representing the keys of the keypad
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte pinRows[NR_ROWS] = {11, 10, 9, 8}; // digital pins representing the rows of a 4x4 keypad
byte pinCols[NR_COLS] = {7, 6, 5, 4};   // digital pins representing the columns of a 4x4 keypad

Keypad custom_keypad = Keypad(makeKeymap(keys),
                              pinRows,  // assign pins used for rows
                              pinCols,  // assign pins used for cols
                              NR_ROWS,  // the number of rows
                              NR_COLS); // the number of cols

/* Setup function */
void setup()
{
    Serial.begin(9600); // to print to output
    Serial.println("Reading keypresses...\n");
}

/* Loop function */
void loop()
{
    char key = custom_keypad.getKey(); // read input from the buttons of the keypad

    if (key)                           // check for any characters that have been pressed
    { 
        Serial.println(key);
    }
}
