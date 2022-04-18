// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
#include <Arduino.h>

int x_ard, y_ard, heading_ard, id_ard;

int x_destination[] = {0};
int y_destination[] = {0};
int heading_destination[] = {0};

int x_offset, y_offset = 0;

int samplestored[5];
int numberofsamplestored = 0;

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
// Start indien niets gevonden wordt
// Wanneer iets gedropt is, niet direct opnieuw targeten met de camera!
