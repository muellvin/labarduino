//derived from the Demo Code:
// Demo code for Grove - Temperature Sensor V1.1/1.2; Loovee @ 2015-8-26

#include <math.h>

const int B = 4275;               // B value of the thermistor
//using the mean of the two boundaries given in the lab tutorial
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0




void setup()
{
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

    Serial.print("temperature = ");
    Serial.println(temperature);

    delay(1000);
}