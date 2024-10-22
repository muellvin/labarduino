#include <rgb_lcd.h>
#include <math.h>

//libraries for LCD display
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

// Include PID Library
#include <PID_v1.h>        


#define BCircuit 5831             // B value of the circuit's thermistor

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




// PID parameters
double Setpoint, Input, Output;
double Kp = 4000, Ki = 50000, Kd = 5000; // Tweak these values to tune the PID


//Digital LowPassFilter
double temp_before = 25;


// Create PID object
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);

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



    // Set initial setpoint temperature (e.g., 25°C)
    Setpoint = 25.0;
    
    // Initialize the PID loop
    myPID.SetMode(AUTOMATIC);
    
    // Set PWM output limits (0 - 255 for controlling fan speed)
    myPID.SetOutputLimits(0, 255);
    
    // Set PID sample time (in milliseconds)
    myPID.SetSampleTime(100); // Adjust as needed

}

void loop()
{
  
    

    //PID input
    Input = read_temp(0.1);

    int potValue = analogRead(pinPotentiometer);
    int upperthold = map(potValue, 0, 1023, 0, 50);
    int lowerthold = upperthold-4;
    Setpoint = upperthold -2;


    // Compute PID Output
    myPID.Compute();

    if (Output != 0 && Output <50){
      Output = 50;
    }
    analogWrite(pinFanControl, Output);



    //fan speed reading; 60 seconds in a minute
    int rpm = (pulseCount * 60) / pulsesPerRevolution;
    pulseCount = 0;  // Reset the pulse count for the next measurement period
    
    


    


    //lcd

    if (Input >= upperthold) {
      lcd.setRGB(255, 0, 0);  
    } 
    else if (Input <= lowerthold) {
      lcd.setRGB(255, 0, 0); 
    }
    else{
      lcd.setRGB(0, 255, 0); 
    }

    // Use dtostrf() to convert float to string
    char TempBufferCircuit[10];
    dtostrf(Input, 6, 2, TempBufferCircuit); 
    // "6" is total width, "2" is decimal places

    lcd.clear();
    lcd.write("Temp: ");
    lcd.write(TempBufferCircuit);

    lcd.setCursor(0, 1);  // Move to the second row
    lcd.print("Goal: ");
    lcd.print(Setpoint);


    


    

    //Serial prints

    //print to Serial for plotting or saving
    Serial.print(Input);
    Serial.print('\t');
    Serial.print(Setpoint);
    Serial.print('\t');
    Serial.println(Output);




    
    delay(100);
}

// Interrupt Service Routine (ISR) to count pulses
void countPulse() {
  pulseCount++;  
}


float read_temp(float alpha) {
    int VCircuit = analogRead(pinTempCircuit);
    //equation (8) used to compute the resistance
    float RCircuit = (1023.0/VCircuit-1.0);
    //equation (9) to compute the temperature from the resistor
    float TempCircuit = 1.0/(log(RCircuit)/BCircuit+1/298.15)-273.15; 

    float temperature = alpha * TempCircuit + (1-alpha)*temp_before;
    temp_before = temperature;
    return temperature;

}