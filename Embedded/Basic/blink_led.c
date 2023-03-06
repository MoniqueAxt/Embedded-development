/*
A simple system with a periodically blinking LED
and another LED that is turned on/off when a
button is pressed/released.
*/

// Define section
#define LED_BLINK 13    // digital pin for the blinking LED
#define LED_PUSH 12     // digital pin for the button-activated LED
#define BUTTON 7        // digital pin for the push button

void setup()
{
	pinMode(LED_BLINK, OUTPUT);   // generate an electrical signal
  	pinMode(LED_PUSH, OUTPUT);	
  	pinMode(BUTTON, INPUT);
}
  
void loop() 
{
  // blink an LED
  if (digitalRead(BUTTON) == HIGH){   // if the button is pressed
    digitalWrite(LED_PUSH, HIGH);     // turn on the LED
  }
  else {
    digitalWrite(LED_PUSH, LOW);      // else turn it off
  }
  
  // activate an LED on button push
  digitalWrite(LED_BLINK, HIGH);      // turn on the blinking LED
  delay(400);                         // blink for 400ms
  digitalWrite(LED_BLINK, LOW);       // turn off the blinking LED
  delay(600);                         // pause for 600ms
  
}