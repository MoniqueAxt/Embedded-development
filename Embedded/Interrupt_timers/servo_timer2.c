/* 
Analog timer interrupts used to move a Servo motor once every second.
*/

// Include section
#include <Servo.h>

// Define section
#define SERVO_PIN A0

// (timer speed (Hz)) = (Arduino clock speed (16MHz)) / PRESCALER)
#define PRESCALER 1024          // used to set the speed of the timer (divides down the clock signals used for the timer, giving reduced overflow rates).
#define INTERRUPT_FREQ 125      // define the interrupt frequency
#define CLOCK_SPEED 16000000    // clock speed of the Arduino Uno
#define TIMER_2_MAX 256         // the max counter value for timer 2
#define TIMER_2_MIN 0           // the min counter value for timer 2

// Globals
Servo servo;
int iSeconds = 0;
int ticks = 0;
int timer2_CMR = 0;

/* Setup function */
void setup() {
 Serial.begin(9600);
 Serial.println("---Starting---");
 servo.attach(SERVO_PIN);     // set up the servo motor
   
  setup_timer_1();
}

void loop() {
   
}

/* Set up timer 2  */
void setup_timer_1() {
  cli();       // stop interrupts

  TCCR2A = 0;  // set the entire TCCR2A register to 0 (PWM - not used so set to 0)
  TCCR2B = 0;  // set the entire TCCR2B register to 0 (for setting prescaler value)
  TCNT2 = 0;   // initialise the timer 2 counter value to 0

  // calculate at which point to stop the counter
  timer2_CMR = (CLOCK_SPEED / ((long)(PRESCALER) * INTERRUPT_FREQ)) - 1;
  Serial.print("timer2_CMR: ");
  Serial.println(timer2_CMR);
  
  if (timer2_CMR >= TIMER_2_MAX) {          // if the value of timer2's CMR is above the max value
    timer2_CMR = TIMER_2_MAX;               // set the timer to the max value
     Serial.println((String) "timer2's CMR was set too high and was changed to: " + timer2_CMR);// notify user
  }
  else if (timer2_CMR <= TIMER_2_MIN) {    // if the value of timer2's CMR is below the min value
    timer2_CMR = TIMER_2_MIN;              // set the timer to the min value
    Serial.println((String) "timer2's CMR was set too low and was changed to: " + timer2_CMR); // notify user
  }
  
   OCR2A = timer2_CMR;                     // set compare match register
   TCCR2A |= (1 << WGM21);                // turn on CTC mode for Timer2
   TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // set CS22, 21 and 20 for a 1024 prescaler 
   TIMSK2 |= (1 << OCIE2A);               // enable timer compare interrupt

  sei();                                  // allow interrupts
}

/* The Interrupt Service Routine for timer 2 */
ISR(TIMER2_COMPA_vect) {
  cli();                                // stop/clear other interrupts - get exclusive access
  
  ticks++;                              // increment the tick counter

  if (ticks == INTERRUPT_FREQ){         // 1 second passes after INTERRUPT_FREQ number of ticks                                        
   servo.write(iSeconds++);
   Serial.println(iSeconds);
   ticks = 0;                          // reset the tick counter
  }
  
  sei();                                // allow interrupts - release exclusive access
}