#include <math.h>

// Detect if a garage door is open using a magnet door switch.
// Put the switch on the door, send some voltage through it,
// if we get voltage then the door is closed.  If there's nothing
// then the door is open.  Then we can send this information somewhere
// to blink a status light in the house or something.
//
// I personally want this on my way to bed so I don't have to say DOH
// in the morning.

int led1 = D7;
int door = A0;
int sensorValue = 0;
double voltage;

// json variables
char *door_open;

// let's do some event faking so we don't spam particle.io
// track the previous state with some ints.  then we'll compare
// and send an event if it doesn't match the current state.
// set to an invalid state first
int previousState;
int doorState;
const int GARAGE_CLOSED = 0;
const int GARAGE_OPEN = 1;

void garageOpenedEvent() {
  Spark.publish("garage/open", "true", 60, PRIVATE);
}

void garageClosedEvent() {
  Spark.publish("garage/open", "false", 60, PRIVATE);
}

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(door, INPUT);

  Spark.variable("garage/voltage", &sensorValue, DOUBLE);
  Spark.variable("garage/door_open", &door_open, STRING);
  readDoorSensor();
  previousState = doorState;
}

void readDoorSensor() {
  sensorValue = analogRead(door);
  voltage = ((sensorValue * 3.3) / 4095);

  // 3 is a magic number here, coresponds to the voltate I saw with Serial.println()
  if (voltage > 3) {
    doorState = GARAGE_CLOSED;
  } else {
    doorState = GARAGE_OPEN;
  }
}

void debugLight() {
  if (doorState == GARAGE_OPEN) {
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }
}

bool eventHasHappened() {
  if (previousState != doorState) {
    previousState = doorState;
    return true;
  } else {
    return false;
  }
}

void sendEvent() {
  if (doorState == GARAGE_OPEN) {
    door_open = "true";
  } else {
    door_open = "false";
  }

  String roundedVoltage = doubleToString(voltage, 2);

  // create a json document by hand because string support is weak here
  String json = "{\"door_open\":";
  json.concat(door_open);
  json.concat(", \"voltage\":");
  json.concat(roundedVoltage);
  json.concat("}");

  Spark.publish("garage/open", json, 60, PRIVATE);
}

void loop() {
  readDoorSensor();
  debugLight();

  if (eventHasHappened()) {
    sendEvent();
  }

  delay(5000);
}

String doubleToString(double input,int decimalPlaces){
  if(decimalPlaces!=0){
    String string = String((int)(input*pow(10,decimalPlaces)));
    if(abs(input)<1){
      if(input>0)
        string = "0"+string;
      else if(input<0)
        string = string.substring(0,1)+"0"+string.substring(1);
    }
    return string.substring(0,string.length()-decimalPlaces)+"."+string.substring(string.length()-decimalPlaces);
  }
  else {
    return String((int)input);
  }
}
