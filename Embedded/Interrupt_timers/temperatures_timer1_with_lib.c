/* 
Temperature measurements displayed via multiple LEDs using interrupts with timer 1.
Using TIMERONE library: https://code.google.com/archive/p/arduino-timerone/
*/

// Include section
#include <TimerOne.h>

// Define section
#define LED_r1 13             // LED used for range 1 temperatures
#define LED_r2 12             // LED used for range 2 temperatures
#define LED_r3 11             // LED used for range 3 temperatures
#define LED_r4 10             // LED used for range 4 temperatures
#define LED_r5 9              // LED used for range 5 temperatures
#define TEMP_SENSOR A0        // the temperature sensor pin

// Constants section
const int R_MIN = -41;        // the minimum temperature that is considered
const int R_MAX = 125;        // the maximum temperature that is considered
const int R1_MAX = 9;         // upper bound on range 1 temperature
const int R2_MAX = 15;        // upper bound on range 2 temperature
const int R3_MAX = 20;        // upper bound on range 3 temperature
const int R4_MAX = 25;        // upper bound on range 4 temperature
const int NR_RANGES = 5;      // the number of temperature ranges used
// 2d array holding the max temp of each range and their corresponding LED pins
const int temp_ranges_pins[][NR_RANGES] ={ 
   {R1_MAX, LED_r1},          // max temp for range 1, LED pin for range 1
   {R2_MAX, LED_r2},          // max temp for range 2, LED pin for range 2
   {R3_MAX, LED_r3},          // max temp for range 3, LED pin for range 3
   {R4_MAX, LED_r4},          // max temp for range 4, LED pin for range 4
   {R_MAX, LED_r5}            // max temp for range 5, LED pin for range 5
};


void setup(){ 
   Serial.begin(9600);
   Timer1.initialize(1000000);  //Initialise timer with a 1 second period
   Timer1.attachInterrupt(callback);
}

void loop() {}

/* Measure the temperature and activate an LED based on the measurement */
void callback() {
  float current_tempC = get_temperature(analogRead(TEMP_SENSOR));  // measure the temperature and convert it to celsius
  print_to_serial("Temperature: ", current_tempC, "\262C");
  int i;             // counter to iterate through the 2d array
  size_t size = sizeof(temp_ranges_pins) / sizeof(temp_ranges_pins[0]); // the size of the 2d array
  int temp_before;   // the max temperature in the previous range
  int temp_after;    // the max temperature in the current range
  
  // iterate through each temperature range to find a match
  for (i = 0; i < size; i++) {
   if (i == 0) {                                // if we're checking range 1 temperatures
      temp_before = R_MIN;                      // no range before this - compare to the absolute minimum temp
      temp_after = temp_ranges_pins[i][0];      // get the max temp in this range
   }
   else if (i == size-1) {                      // if we're checking the last range temperatures
      temp_before = temp_ranges_pins[i - 1][0]; // get the max temp in the previous range
      temp_after = R_MAX;                       // no range after this - compare to the absolute maximum temp
   }
   else {                                       // if we're checking any other temperature ranges
      temp_before = temp_ranges_pins[i - 1][0]; // get the max temp of the previous range
      temp_after = temp_ranges_pins[i][0];      // get the max temp of this range
   }      

   // check which range the current temperature falls into and activate an LED
    if (current_tempC > temp_before && current_tempC <= temp_after) { // if the measured temp IS in this range
      digitalWrite(temp_ranges_pins[i][1],HIGH);  // activate the LED pin associated with this temp range
      print_to_serial("\tTurn on LED: ", i+1, "");   
    } 
    else {                                        // if the measured temp is NOT in this range
      digitalWrite(temp_ranges_pins[i][1], LOW);  // turn off the LED pin associate with this temp range
    }
     delay(400);
  }
}

/* Convert the raw analog temperature value to celisius */
float get_temperature(int rawADC_value) {
  float voltage = (rawADC_value * 5000.0) / 1024;  // convert the raw sensor value to millivolts
  voltage = voltage - 500;                         // remove the voltage offset
  int temperature = voltage / 10;                  // convert millivolts to Celsius
  return temperature;
}

/* Print a message and a value to the serial monitor*/
void print_to_serial(char* msg, int i, char* unit) {
  Serial.print(msg);     // print a leading message before the value
  Serial.print(i);       // print the value
  Serial.println(unit);  // print the measurement unit
}