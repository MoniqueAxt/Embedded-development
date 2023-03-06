/*
A system that displays the relationship between the measured temperature and light intensity.
Different LEDs will blink depending on how closely the light intensity matches the temperature.
The current temperature is displayed on a LCD screen.
*/

// Include section
#include <LiquidCrystal.h>

// Define section
#define LED_NORMAL 13             // GREEN LED for normal dependency
#define LED_LOW 12                // BLUE LED for LOW temp
#define LED_HIGH 11               // RED LED for HIGH temp
#define TEMPERATURE_PIN A0        // temperature sensor pin
#define LDR_PIN A1                // light-dependent resistor pin

#define RS 10					// set the LCD's Register Select pin
#define E 9						// set the LCD's Enable pin
#define D4 5					// set the data pin
#define D5 4					// set the data pin
#define D6 3					// set the data pin
#define D7 2					// set the data pin

// Set up the LCD
LiquidCrystal lcd(RS,E, D4,D5,D6,D7);

// const variables for the light measurement
const int LDR_MIN = 0;            // the min value the LDR emits in lumens
const int LDR_MAX = 2086;         // the max value the LDR emits in lumens
const double VIN = 5.0;           // power voltage     
const double k = VIN/1024;        // 5V split into 1024 values by ADC   
const double R2 = 10000;          // resister's value in ohms
const double B = 1.3*pow(10.0,7); // 1.3*10^7
const double m = -1.4;            // negative exponent - light intensity decreases as resistance increases

/* Get the analog light sensor resistance reading in voltage and calculate the lux from that value. */
double get_light_intensity(int rawADC_value) {
  double V2 = k * rawADC_value;   // V2 = I*R2
  double R1 = (VIN/V2 - 1) * R2;  // R = V/I
  double lux = B*pow(R1, m);      // B*R1^m
  return lux;                     // return lux measured in lumens
}

/* Get the analog temperature reading and convert it to celisius */
float get_temperature(int rawADC_value) {
  float voltage = (rawADC_value * 5000.0) / 1024;   // convert the raw sensor value to millivolts
  voltage = voltage - 500;                          // remove the voltage offset
  int temperature = voltage / 10;                   // convert millivolts to Celsius
  return temperature;
}
/* Map the given value to a percentage scale (0-100%) */
float get_light_percentage(double value) {
  return map(value, LDR_MIN, LDR_MAX, 0, 100);      // map the value to a range 0 - 100
}

/* Print a message and a value to the serial monitor*/
void print_to_serial(char *msg, int i, char* unit){
  Serial.print(msg);      // print a leading message before the value
  Serial.print(i);        // print the value 
  Serial.println(unit);   // print the measurement unit 
}

/* Compare the light intensity and temperature and blink an LED depending on whether the
dependency is normal, or the temp is too high, or the temp is too low - given the current light intensity */
void measure_and_show_dependecy(int light_percentage, int temp_celsius){
  // save the different categories of light intensity
  int light_range1 = (light_percentage == 0);                              // light intensity is 0%
  int light_range2 =  (light_percentage >= 1 && light_percentage <= 20);   // light intensity up to 20%
  int light_range3 = (light_percentage >= 21 && light_percentage <= 60);   // light intensity up to 60%
  int light_range4 = (light_percentage >= 61 && light_percentage <= 100);  // light intensity up to 100%
  
  // determine if the temperature is too low
  int temp_too_low = 
  ( (light_range2) && (temp_celsius < -12) )  ||    // temp is below -12
  ((light_range3) && (temp_celsius < 0) )     ||    // temp is below 0
  ( (light_range4) && (temp_celsius < 21) );        // temp is below 21
  // determine if the temperature is too high
  int temp_too_high = 
  ( (light_range1) && (temp_celsius > -12) )  ||    // temp is above -12
  ( (light_range2) && (temp_celsius > 0) )    ||    // temp is above 0
  ((light_range3) && (temp_celsius > 20) );         // temp is above 20

  // the temperature is too low given the light intensity
  if (temp_too_low) {			          
      digitalWrite(LED_LOW, HIGH);	  // turn on the LED representing LOW temperature
      Serial.println("-------------LOW: blue");
      delay(500);					
      digitalWrite(LED_LOW, LOW);	    //  turn off the LED representing LOW temperature
  }
  // the temperature is too high given the light intensity
  else if (temp_too_high) {	        
      digitalWrite(LED_HIGH, HIGH);	// turn on the LED representing HIGH temperature
      Serial.println("-------------HIGH: red");
      delay(500);				
      digitalWrite(LED_HIGH, LOW);	// turn off the LED representing HIGH temperature
  }
  // the dependency between light intensity and temperature is normal
  else {								
      digitalWrite(LED_NORMAL, HIGH);	// turn on the LED representing NORMAL dependency
      Serial.println("-------------NORMAL: green");
      delay(500);				
      digitalWrite(LED_NORMAL, LOW);	// turn off the LED representing NORMAL dependency
  }
}

/* Setup function */
void setup()
{
  pinMode(LED_NORMAL, OUTPUT);
  pinMode(LED_LOW, OUTPUT);
  pinMode(LED_HIGH, OUTPUT);
  pinMode(LDR_PIN, INPUT);    // get data from the light sensor

  lcd.begin(16,2);            // set up the number of cols and rows on the LCD
  Serial.begin(9600);         // start the serial   
}

/* Loop function */
void loop()
{
  int temp_sensor_value = analogRead(TEMPERATURE_PIN);        // read the raw temperature sensor value
  float temp_celsius = get_temperature(temp_sensor_value);    // convert the raw value to celsius
  
  // get the analog voltage value (auto-converted to digital value 0-1023)
  int LDR_raw_value = analogRead(LDR_PIN);                    // the raw value read from the LDR sensor
  double lumens = get_light_intensity(LDR_raw_value);         // convert the raw LDR value into lumens
  int light_percentage = get_light_percentage(lumens);        // convert the lumens to a percentage
   
  print_to_serial("Temp : ", temp_celsius, " C");
  print_to_serial("Light: ", light_percentage, "%");
  
  lcd.print("Temperature");
  lcd.setCursor(0,1);                           // set the cursor to col 0, row 1
  lcd.print(temp_celsius);	                    // display the temperature on the LCD
  lcd.print("\262C");                           // degree symbol and C

  
  // activate a particular LED based on the dependency values
  // between the light intensity and temperature
  measure_and_show_dependecy(light_percentage, temp_celsius);
 
  Serial.println("");
  delay(1000);
  lcd.clear();
}
