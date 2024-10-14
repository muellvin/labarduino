//Pin for the fan control
const int pinFanControl = 11;

#define BCircuit 5831             // B value of the circuit's thermistor
const int pinTempCircuit = A2;    // circuit reading


//for fan sensing
const int pinFanSense = 2;
bool fanOn = false;
volatile int pulseCount = 0;      // Variable to count the number of pulses
const int pulsesPerRevolution = 2; // Number of pulses per revolution for your fan (commonly 2 or 4)

int dutycicle = 5;
//interval when the rpm is read; not equal to loops to not get overflow in rotation count
const int interval = 1;
int loop_count= 0;

//how many seconds between the reading
const int loops = 120;

void setup() {
  // put your setup code here, to run once:
  //setting up the communication channel
  Serial.begin(9600);

  pinMode(pinFanControl, OUTPUT);
  pinMode(pinFanSense, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinFanSense), countPulse, FALLING); // Interrupt on pulse falling edge
  Serial.begin(9600);

}

void loop() {
  //fan speed reading
  int rpm = (pulseCount * 60/interval) / pulsesPerRevolution;
  pulseCount = 0;  // Reset the pulse count for the next measurement period
  
  analogWrite(pinFanControl, dutycicle);

  //temperature
  int VCircuit = analogRead(pinTempCircuit); 
  float RCircuit = (1023.0/VCircuit-1.0);
  float TempCircuit = 1.0/(log(RCircuit)/BCircuit+1/298.15)-273.15; 

  char TempBufferCircuit[10];
  dtostrf(TempCircuit, 6, 2, TempBufferCircuit); 

  if (loop_count == loops){
    Serial.print(rpm);
    Serial.print('\t');
    Serial.print(dutycicle);
    Serial.print('\t');
    Serial.println(TempBufferCircuit);
    

    if (dutycicle <255){
      dutycicle += 10;
    }

    loop_count = 0;
}
  
  

  loop_count +=1;
  
  delay(interval*1000);

}

// Interrupt Service Routine (ISR) to count pulses
void countPulse() {
  pulseCount++;  
}
