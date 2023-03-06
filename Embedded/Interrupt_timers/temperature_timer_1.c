/* 
Temperature measurements displayed via multiple LEDs using interrupts with timer 1 
(No timer library used)
*/

#define LED_r1 13             // LED used for range 1 temperatures
#define LED_r2 12             // LED used for range 2 temperatures
#define LED_r3 11             // LED used for range 3 temperatures
#define LED_r4 10             // LED used for range 4 temperatures
#define LED_r5 9              // LED used for range 5 temperatures
#define TEMP_SENSOR A0        // the temperature sensor pin
// (timer speed (Hz)) = (Arduino clock speed (16MHz)) / PRESCALER)
#define PRESCALER 1024        // used to set the speed of the timer (divides down the clock signals used for the timer, giving reduced overflow rates).
#define INTERRUPT_FREQ 1      // define the interrupt frequency
#define CLOCK_SPEED 16000000  // clock speed of the Arduino Uno
#define TIMER_1_MAX 65535     // the max counter value for timer1
#define TIMER_1_MIN 256       // the min counter value for timer 1

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

int timer1_CMR = 0;  // compare match register value for timer 1

void setup() {
  // set up the LEDs
  pinMode(LED_r1, OUTPUT);
  pinMode(LED_r2, OUTPUT);
  pinMode(LED_r3, OUTPUT);
  pinMode(LED_r4, OUTPUT);
  pinMode(LED_r5, OUTPUT);

  Serial.begin(9600);
  Serial.println("---Starting---");

  setup_timer_1();   // setup timer 1
}

void loop() {}

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

/* Set up timer 1  */
void setup_timer_1() {
  cli();       // stop interrupts

  TCCR1A = 0;  // set the entire TCCR1A register to 0
  TCCR1B = 0;  // set the entire TCCR1B register to 0
  TCNT1 = 0;   // initialise the timer 1 counter value to 0

  // calculate at which point to stop the counter
  timer1_CMR = (CLOCK_SPEED / (PRESCALER * INTERRUPT_FREQ) - 1);

  if (timer1_CMR >= TIMER_1_MAX) {        // if the value of timer1's CMR is above the max value
    timer1_CMR = TIMER_1_MAX;             // set the timer to the max value
     Serial.println((String) "timer1 was set too high and was changed to: " + timer1_CMR);// notify user
  }
  else if (timer1_CMR <= TIMER_1_MIN) {    // if the value of timer1's CMR is below the min value
    timer1_CMR = TIMER_1_MIN;              // set the timer to the min value
    Serial.println((String) "timer1 was set too low and was changed to: " + timer1_CMR); // notify user
  }

   OCR1A = timer1_CMR;                  // set compare match register for 1hz increments
   TCCR1B |= (1 << WGM12);              // turn on CTC mode
   TCCR1B |= (1 << CS12) | (1 << CS10); // set CS10 and CS12 bits for 1024 prescaler  
   TIMSK1 |= (1 << OCIE1A);             // enable timer compare interrupt

   Serial.println("Timer setup done");
   sei();  // allow interrupts
}

/* The Interrupt Service Routine for timer 1 */
ISR(TIMER1_COMPA_vect) {
  cli();  // stop/clear other interrupts - get exclusive access
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
      digitalWrite(temp_ranges_pins[i][1], HIGH);  // activate the LED pin associated with this temp range
      print_to_serial("\tTurn on LED: ", i+1, "");   
    } 
    else {                                        // if the measured temp is NOT in this range
      digitalWrite(temp_ranges_pins[i][1], LOW);  // turn off the LED pin associate with this temp range
    }
     delay(400);
  }
  sei();  // allow interrupts - release exclusive access
}