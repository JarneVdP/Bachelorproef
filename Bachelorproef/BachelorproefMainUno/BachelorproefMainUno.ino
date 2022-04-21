// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
#include <Arduino.h>

int x_ard, y_ard, heading_ard, id_ard;

int x_destination[] = {0};
int y_destination[] = {0};
int heading_destination[] = {0};

double delta_x, delta_y, x_pos, y_pos;
float heading;

int x_offset, y_offset = 0;

int samplestored[5];
int numberofsamplestored = 0;

struct PositionStruct{
  double x_pos;
  double y_pos;
  float heading;
};

PositionStruct Position = {0, 0, 0};

void setup() {
  Serial.begin(9600);
  Serial.println("Let's go");
}


void loop() {
  odometry(Position);
  Serial.println(Position.x_pos);
  SerialCommunication();
  int *distanceF = measureDist(); //located in different file 
  if (x_ard > 0 && y_ard > 0) {
    movement();
  }
  Serial.println(x_pos);
}

// Timer 90 seconden
// Start indien niets gevonden wordt
// Wanneer iets gedropt is, niet direct opnieuw targeten met de camera!
