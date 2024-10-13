#include <rgb_lcd.h>

//derived from the Demo Code:
// Demo code for Grove - Temperature Sensor V1.1/1.2; Loovee @ 2015-8-26

#include <math.h>

//libraries for LCD display
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

const int B = 4275;               // B value of the thermistor
//using the mean of the two boundaries given in the lab tutorial
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

//temperature threshold to change display color
float threshold = 30.0;
// Pin for the rotary angle sensor
const int pinPotentiometer = A1;  


void setup()
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    // Set initial backlight color to white
    lcd.setRGB(255, 255, 255);  

    //setting up the communication channel
    Serial.begin(9600);

}

void loop()
{
    int V_meas = analogRead(pinTempSensor);

    //equation (8) used to compute the resistance
    float R = R0*(1023.0/V_meas-1.0);
    //equation (9) to compute the temperature from the resistor
    float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; 


    //rest is printing to LCD:

    // Use dtostrf() to convert float to string
    char tempBuffer[10];  // Create a buffer for the string
    dtostrf(temperature, 6, 2, tempBuffer);  // (float, width, precision, buffer)
    // "6" is total width, "2" is decimal places
    
    lcd.clear();
    lcd.write("Temp: ");
    lcd.write(tempBuffer);


    int potValue = analogRead(pinPotentiometer);
    threshold = map(potValue, 0, 1023, 0, 50);

    lcd.setCursor(0, 1);  // Move to the second row
    lcd.print("Thold: ");
    lcd.print(threshold);

    if (temperature >= threshold) {
    lcd.setRGB(255, 0, 0);  // Set color to red if above or equal to threshold
  } else {
    lcd.setRGB(0, 255, 0);  // Set color to green if below threshold
  }

  //print to Serial for plotting or saving
  Serial.print(tempBuffer);
  Serial.print('\t');
  Serial.println(threshold);


  
    delay(1000);
}