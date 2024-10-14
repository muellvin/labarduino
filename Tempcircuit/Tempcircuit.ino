#include <rgb_lcd.h>

//derived from the Demo Code:
// Demo code for Grove - Temperature Sensor V1.1/1.2; Loovee @ 2015-8-26

#include <math.h>

//libraries for LCD display
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

#define BGrove 4275              // B value of the Grove thermistor
#define BCircuit 5831             // B value of the circuit's thermistor

//using the mean of the two boundaries given in the lab tutorial
//const int R0 = 100000;            // R0 = 100k cancels out
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0
const int pinTempCircuit = A2;    // circuit reading


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

    Serial.println("TGrove \t TCirc \t Pot");

}

void loop()
{
    int VGrove = analogRead(pinTempSensor);
    int VCircuit = analogRead(pinTempCircuit);


    //equation (8) used to compute the resistance
    float RGrove = (1023.0/VGrove-1.0);
    float RCircuit = (1023.0/VCircuit-1.0);
    //equation (9) to compute the temperature from the resistor
    float TempGrove = 1.0/(log(RGrove)/BGrove+1/298.15)-273.15; 
    float TempCircuit = 1.0/(log(RCircuit)/BCircuit+1/298.15)-273.15; 



    // Use dtostrf() to convert float to string
    char TempBufferGrove[10];  // Create a buffer for the string
    char TempBufferCircuit[10];
    dtostrf(TempGrove, 6, 2, TempBufferGrove);  // (float, width, precision, buffer)
    dtostrf(TempCircuit, 6, 2, TempBufferCircuit); 
    // "6" is total width, "2" is decimal places


    
    lcd.clear();
    lcd.write("Temp: ");
    lcd.write(TempBufferGrove);


  int potValue = analogRead(pinPotentiometer);
  int threshold = map(potValue, 0, 1023, 0, 50);

  lcd.setCursor(0, 1);  // Move to the second row
  lcd.print("Thold: ");
  lcd.print(threshold);

  if (TempGrove >= threshold) {
    lcd.setRGB(255, 0, 0);  // Set color to red if above or equal to threshold
  } else {
    lcd.setRGB(0, 255, 0);  // Set color to green if below threshold
  }
  
  //print to Serial for plotting or saving
  Serial.print(TempBufferGrove);
  Serial.print('\t');
  Serial.print(TempBufferCircuit);  
  Serial.print('\t');

  Serial.println(threshold);

  
  delay(1000);
}