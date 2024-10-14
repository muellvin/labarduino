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

//Pin for the fan control
const int pinFanControl = 11;


//for fan sensing
const int pinFanSense = 2;
bool fanOn = false;
volatile int pulseCount = 0;      // Variable to count the number of pulses
const int pulsesPerRevolution = 2; // Number of pulses per revolution for your fan (commonly 2 or 4)



void setup()
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    // Set initial backlight color to white
    lcd.setRGB(255, 255, 255);  

    //setting up the communication channel
    Serial.begin(9600);

    pinMode(pinFanControl, OUTPUT);
    pinMode(pinFanSense, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinFanSense), countPulse, FALLING); // Interrupt on pulse falling edge

}

void loop()
{
  
    int VGrove = analogRead(pinTempSensor);
    int VCircuit = analogRead(pinTempCircuit);

    int potValue = analogRead(pinPotentiometer);
    int upperthold = map(potValue, 0, 1023, 0, 50);
    int lowerthold = upperthold-4;


    //equation (8) used to compute the resistance
    float RGrove = (1023.0/VGrove-1.0);
    float RCircuit = (1023.0/VCircuit-1.0);
    //equation (9) to compute the temperature from the resistor
    float TempGrove = 1.0/(log(RGrove)/BGrove+1/298.15)-273.15; 
    float TempCircuit = 1.0/(log(RCircuit)/BCircuit+1/298.15)-273.15; 

    





    //fan speed reading; 60 seconds in a minute
    int rpm = (pulseCount * 60) / pulsesPerRevolution;
    pulseCount = 0;  // Reset the pulse count for the next measurement period
    
    





    //2 pin control conditions

    if (TempCircuit >= upperthold) {
      lcd.setRGB(0, 255, 0);  
      if (!fanOn){fanOn = true;}
    } 
    if (TempCircuit <= lowerthold) {
      lcd.setRGB(255, 0, 0); 
      if (fanOn){fanOn = false;}
    }
    else{
      lcd.setRGB(0, 255, 0); 
    }
    if (fanOn){
      analogWrite(pinFanControl, 255); 
    }
    else{ 
      analogWrite(pinFanControl,0);
    }





    //lcd prints

    // Use dtostrf() to convert float to string
    char TempBufferGrove[10];  // Create a buffer for the string
    char TempBufferCircuit[10];
    dtostrf(TempGrove, 6, 2, TempBufferGrove);  // (float, width, precision, buffer)
    dtostrf(TempCircuit, 6, 2, TempBufferCircuit); 
    // "6" is total width, "2" is decimal places

    lcd.clear();
    lcd.write("Temp: ");
    lcd.write(TempBufferCircuit);

    lcd.setCursor(0, 1);  // Move to the second row
    lcd.print("Thr: ");
    lcd.print(lowerthold);
    lcd.print("-");
    lcd.print(upperthold);

    


    

    //Serial prints

    //print to Serial for plotting or saving
    //Serial.print(TempBufferGrove);
    //Serial.print('\t');
    //Serial.println(TempBufferCircuit);  
    //Serial.print('\t');

    //Serial.println(threshold);

    Serial.print("Fan Speed: ");
    Serial.print(rpm);
    Serial.println(" RPM");



    
    delay(1000);
}

// Interrupt Service Routine (ISR) to count pulses
void countPulse() {
  pulseCount++;  
}