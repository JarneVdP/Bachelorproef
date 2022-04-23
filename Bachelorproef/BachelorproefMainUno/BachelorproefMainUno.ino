// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
#include <Arduino.h>


// +--------+
// +Odometry+
// +--------+
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

// +-----+
// +Servo+
// +-----+
#include <SoftwareSerial.h>
#include "MeOrion.h"
#include "MeSmartServo.h"
#include <AccelStepper.h>

MeSmartServo mysmartservo(PORT5);   //UART2 is on port 5

long loopTime = 0; 
long carBeforeSample = 0; //car is before sample --> 1 otherwise 0
long carBaseCamp = 0; // car is in base camp --> 1 otherwise 0
bool dir = LOW; //stepper off

const int mtr_speed = 20;
const int dirPin = 2;
const int stepPin = 3;
const int mosfet = 4;

const int correctServ1 = 10;
const int correctServ2 = 10;
const int correctServ3 = 0;



void setup() {
  //Servo
  Serial.begin(115200);
  mysmartservo.begin(115200);
  delay(5);
  mysmartservo.assignDevIdRequest();
  delay(50);
  Serial.println("setup!");
  loopTime = millis();
  pinMode(mosfet,OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
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
  action(); //servo movement
  Serial.println(x_pos);
}

// Timer 90 seconden
// Start indien niets gevonden wordt
// Wanneer iets gedropt is, niet direct opnieuw targeten met de camera!
