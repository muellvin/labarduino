// Pin where the output signal will be generated
const int outputPin = 8;

void setup() {
  pinMode(outputPin, OUTPUT);
}

void loop() {
  // set the pin HIGH (5V)
  digitalWrite(outputPin, HIGH);
  // waiting for 500 milliseconds
  delay(500);
  
  // set the pin LOW (0V)
  digitalWrite(outputPin, LOW);
  // waiting for 500 milliseconds
  delay(500);
}

