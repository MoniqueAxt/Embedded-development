/* 
A system using a NeoPixel LED strip that lights up 
more LEDs the higher the measured temperature. A buzzer
is sounded if the temperature is above or below the thresholds. 
*/

#include <Adafruit_NeoPixel.h>

#define LED_MAX 13         // activated when value is too high
#define LED_MIN 12         // activated when value is too low
#define NEOPIXEL_PIN 8     // neopixel ring pin
#define NUMPIXELS 12       // number of pixels on the neopixel strip
#define BUZZER 10          // piezo speaker pin
#define TEMPERATURE_PIN A0 // temperature sensor pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB);
const uint32_t BLUE = strip.Color(0, 0, 255);
const uint32_t YELLOW = strip.Color(255, 255, 0);
const uint32_t RED = strip.Color(255, 0, 0);
uint32_t color = strip.Color(75, 250, 100);

const int TMP_RAW_MIN = 20;
const int TMP_RAW_MAX = 358;

void setup()
{
   pinMode(LED_MAX, OUTPUT);
   pinMode(LED_MIN, OUTPUT);
   pinMode(BUZZER, OUTPUT);

   strip.begin();           // initialise the NeoPixel library
   strip.show();            // initialise all pixels to 'off'
   strip.setBrightness(40); // 40/255 (about 15%)

   Serial.begin(9600);     // start the serial
}

void loop()
{
   int temp_sensor_value = analogRead(TEMPERATURE_PIN);     // read the raw temperature sensor value
   print_to_serial("raw temp: ", temp_sensor_value, "");
   float temp_celsius = get_temperature(temp_sensor_value); // convert the raw value to celsius
   print_to_serial("c temp  : ", temp_celsius, " C");
  
   float temp_mapped = map_temperature(temp_sensor_value); // get the temp mapped to range 0-NUMPIXELS-1
    print_to_serial("mapped  : ", temp_mapped, "");

   // the temp is at its minimum so turn all neopixel LEDs off
   if (temp_mapped <= 0) {
      strip.clear();  // clear LEDs
   }
   // the temp is above 0
   else{
      // light up the strip according to temp
      for (int i = 0; i < temp_mapped; i++)
      {
         if (i < 4) {
            strip.setPixelColor(i, BLUE);    // the first 4 LEDs on the strip indicate a cool temperature
         }
         else if (i >= 4 && i < 8 ){         
             strip.setPixelColor(i, YELLOW); // the second 4 LEDs on the strip indicate a warm temperature
         }
         else if (i >= 8) {
             strip.setPixelColor(i, RED);    // the last 4 LEDs on the strip indicate a hot temperature
         }
      }         
   }
   strip.show();
   delay(40);

   // the max temperature that can be displayed on the neopixel ring has been reached
   if (temp_mapped >= NUMPIXELS)
   {
      digitalWrite(LED_MAX, HIGH);  // activate the LED
      tone(BUZZER, 1000);           // activate the buzzer: send 1KHz sound signal
      delay(300);
      noTone(BUZZER);               // stop the sound signal
   }
   // the min temperature that can be displayed on the neopixel ring has been reached
   else if(temp_mapped <= 0){
      digitalWrite(LED_MIN, HIGH);  // activate the LED
      tone(BUZZER, 1000);           // activate the buzzer: send 1KHz sound signal
      delay(300);
      noTone(BUZZER);               // stop the sound signal
   }
   else  // the temperature is within the range the neopixel can represent
   {
      digitalWrite(LED_MAX, LOW);   // turn off the low LED
      digitalWrite(LED_MIN, LOW);   // turn off the high LED
   }

   strip.clear();  // clear LEDs
   Serial.println("-----");
   delay(2000);
}

/* Convert the raw analog temperature value to celisius */
float get_temperature(int rawADC_value)
{
   float voltage = (rawADC_value * 5000.0) / 1024; // convert the raw sensor value to millivolts
   voltage = voltage - 500;                        // remove the voltage offset
   int temperature = voltage / 10;                 // convert millivolts to Celsius
   return temperature;
}

/* Map the given value to a percentage scale (0-100%) */
float map_temperature(double value)
{
   return map(value, TMP_RAW_MIN, TMP_RAW_MAX, 0, NUMPIXELS); // map the value to a range 0-(num pixels on NeoPixel strip)
}

/* Print a message and a value to the serial monitor*/
void print_to_serial(char *msg, int i, char *unit)
{
   Serial.print(msg);    // print a leading message before the value
   Serial.print(i);      // print the value
   Serial.println(unit); // print the measurement unit
}