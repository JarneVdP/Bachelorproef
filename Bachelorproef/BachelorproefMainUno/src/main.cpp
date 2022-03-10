#include <Arduino.h>
#include "Encoder.h"
#include "US_sensor.h"

void setup() {
  Serial.begin(9600);
  
  pinMode(encoderFront, INPUT);
  pinMode(encoderLeft, INPUT);
  pinMode(encoderRight, INPUT);

  pinMode(URsensorFrontTrigger1, OUTPUT); 
  pinMode(URsensorFrontEcho1, INPUT); 
  pinMode(URsensorFrontTrigger2, OUTPUT); 
  pinMode(URsensorFrontEcho2, INPUT); 
}

void loop() {
  // zie encoder.cpp voor volledigere shit
  Position();
  //Get URsensorData -> no collision
  int *distanceF = measureDist(); //located in different file 
  Serial.print("test");
  Serial.print(*distanceF);
}

