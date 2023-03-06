/*
Basic keypad (without external libraru)
*/

// Define section
#define NR_ROWS 4

// Constant section
const int pinRows[4] = {11, 10, 9, 8};      // digital pins representing the rows of a 4x4 keypad
const int pinCols[4] = {7, 6, 5, 4};        // digital pins representing the columns of a 4x4 keypad

/*
  Check if a keypad button was pressed and print the value
*/
void printKeyPressed()
{
    // row 1
    digitalWrite(pinRows[0], LOW);      // send a signal through row 1
    int r1c1 = digitalRead(pinCols[0]); // read the column
    int r1c2 = digitalRead(pinCols[1]); // read the column
    int r1c3 = digitalRead(pinCols[2]); // read the column
    int r1c4 = digitalRead(pinCols[3]); // read the column
    digitalWrite(pinRows[0], HIGH);     // reset row 1
    // row2
    digitalWrite(pinRows[1], LOW);      // set row 2
    int r2c1 = digitalRead(pinCols[0]); // check column 1
    int r2c2 = digitalRead(pinCols[1]); // check column 2
    int r2c3 = digitalRead(pinCols[2]); // read the column
    int r2c4 = digitalRead(pinCols[3]); // read the column
    digitalWrite(pinRows[1], HIGH);     // reset row 2
    // row3
    digitalWrite(pinRows[2], LOW);      // set row 3
    int r3c1 = digitalRead(pinCols[0]); // check column 1
    int r3c2 = digitalRead(pinCols[1]); // check column 2
    int r3c3 = digitalRead(pinCols[2]); // read the column
    int r3c4 = digitalRead(pinCols[3]); // read the column
    digitalWrite(pinRows[2], HIGH);     // reset row 3
    // row4
    digitalWrite(pinRows[3], LOW);      // set row 4
    int r4c1 = digitalRead(pinCols[0]); // check column 1
    int r4c2 = digitalRead(pinCols[1]); // check column 2
    int r4c3 = digitalRead(pinCols[2]); // read the column
    int r4c4 = digitalRead(pinCols[3]); // read the column
    digitalWrite(pinRows[3], HIGH);     // reset row 4

    //=========== ROW 1 =====================
    // we check if 1 was pressed
    if (r1c1 == 0)
    {
        Serial.println('1'); // print 1 to the serial port
        delay(1000);         // delay 1 second
    }
    // check if 2 was pressed
    if (r1c2 == 0)
    {
        Serial.println('2'); // print 2 to serial port
        delay(1000);
    }
    // check if 3 was pressed
    if (r1c3 == 0)
    {
        Serial.println('3'); // print 3 to the serial port
        delay(1000);
    }
    // check if A was pressed
    if (r1c4 == 0)
    {
        Serial.println('A'); // print A to the serial port
        delay(1000);
    }

    //=========== ROW 2 =====================
    // check if 4 was pressed
    if (r2c1 == 0)
    {
        Serial.println('4'); // print 4 to the serial port
        delay(1000);
    }
    // check if 5 was pressed
    if (r2c2 == 0)
    {
        Serial.println('5'); // print 5 to the serial port
        delay(1000);
    }
    // check if 6 was pressed
    if (r2c3 == 0)
    {
        Serial.println('6'); // print 6 to the serial port
        delay(1000);
    }
    // check if B was pressed
    if (r2c4 == 0)
    {
        Serial.println('B'); // print B to the serial port
        delay(1000);
    }

    //=========== ROW 3 =====================
    // check if 7 was pressed
    if (r3c1 == 0)
    {
        Serial.println('7'); // print 7 to the serial port
        delay(1000);
    }
    // check if 8 was pressed
    if (r3c2 == 0)
    {
        Serial.println('8'); // print 8 to the serial port
        delay(1000);
    }
    // check if 9 was pressed
    if (r3c3 == 0)
    {
        Serial.println('9'); // print 9 to the serial port
        delay(1000);
    }
    // check if C was pressed
    if (r3c4 == 0)
    {
        Serial.println('C'); // print C to the serial port
        delay(1000);
    }
    //=========== ROW 4 =====================
    // check if * was pressed
    if (r4c1 == 0)
    {
        Serial.println('*'); // print * to the serial port
        delay(1000);
    }
    // check if 0 was pressed
    if (r4c2 == 0)
    {
        Serial.println('0'); // print 0 to the serial port
        delay(1000);
    }
    // check if # was pressed
    if (r4c3 == 0)
    {
        Serial.println('#'); // print # to the serial port
        delay(1000);
    }
    // check if D was pressed
    if (r4c4 == 0)
    {
        Serial.println('D'); // print D to the serial port
        delay(1000);
    }
}

/* Setup function */
void setup()
{
    // configure the IO ports
    for (int i = 0; i < NR_ROWS; i++)
    {
        pinMode(pinCols[i], INPUT);         // set the columns as input (receive signal)
        pinMode(pinRows[i], OUTPUT);        // set the rows as output (send signal)
    }
    Serial.begin(9600);                     // to print to output
    Serial.println("Reading keypresses...\n");
}

/* Loop function */
void loop()
{
    printKeyPressed(); // call the function that checks if a keypad button was pressed
}
