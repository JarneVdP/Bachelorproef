// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
#include <Arduino.h>

int x_ard, y_ard, heading_ard, id_ard;


void setup() {
  Serial.begin(9600);
  Serial.println("Let's go");
}


void loop() {
  odometry();
  SerialCommunication();
  int *distanceF = measureDist(); //located in different file 
  if (x_ard > 0 && y_ard > 0) {
    movement();
  }
}

// Timer 90 seconden
// posities x, y heading om af te zetten
// offset_x offset_y van de robotarm
// opslaan wat is ingeladen 
//Start indien niets gevonden wordt

