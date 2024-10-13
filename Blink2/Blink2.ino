/*
Program that makes two LED blink with different intervals
time variable overflow is of no concern as differences of unsigned values are used
*/

const int outputPin1 = 8;
const int outputPin2 = 9;

unsigned long time;
unsigned long old_time1 = 0;
unsigned long old_time2 = 0;

unsigned long interval1 = 500; //500 miliseconds (lower numbers than micros)
unsigned long interval2 = 500; //500 miliseconds (lower numbers than micros)

bool LED_state1 = HIGH;
bool LED_state2 = HIGH;


void setup() {
  // put your setup code here, to run once:
  pinMode(outputPin1, OUTPUT);
  pinMode(outputPin2, OUTPUT);


  //for printing debugging into the terminal
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  time = millis();

  //LED 1
  if (time - old_time1 >= interval1){
    LED_state1 = !LED_state1;
    old_time1 = time;
    digitalWrite(outputPin1, LED_state1);

    //printing the state for debugging purposes
    Serial.print(LED_state1);
  }

  //LED 2
  if (time - old_time2 >= interval2){
    LED_state2 = !LED_state2;
    old_time2 = time;
    digitalWrite(outputPin2, LED_state2);
  }

  
}
