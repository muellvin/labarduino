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


// unnecessary makros for code to work on different types of arduinos
#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif

void setup()
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

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


    //printing temperature to the LCD screen

    // Use dtostrf() to convert float to string
    char tempBuffer[10];  // Create a buffer for the string
    dtostrf(temperature, 6, 2, tempBuffer);  // (float, width, precision, buffer)
    // "6" is total width, "2" is decimal places
    
    lcd.clear();
    lcd.write("temp=");
    lcd.write(tempBuffer);


  
    delay(1000);
}