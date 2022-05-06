// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
#include <Arduino.h>

// +------+
// +Motors+
// +------+
//Motor A
int enA = 8;
int in1 = 9;
int in2 = 7;
//Motor B
int enB = 6;
int in3 = 5;
int in4 = 4;

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
bool dir = LOW; //stepper off

const int mtr_speed = 20;
const int dirPin = 43;
const int stepPin = 45;
const int mosfet = 47;

const int correctServ1 = 10;
const int correctServ2 = -45+10;
const int correctServ3 = 0;
//end servo


//###########
//#US Sensor#
//###########
//Pinnen voor ultrasonic sensor
//front
static const int USsensorFrontTrigger1 = 4;
static const int USsensorFrontEcho1 = 5;
static const int USsensorFrontTrigger2 = 8;
static const int USsensorFrontEcho2 = 9;
//left
static const int USsensorLeftTrigger1 = 12;
static const int USsensorLeftEcho1 = 13;
int leftDistance = 0;
//right
static const int USsensorRightTrigger1 = 16;
static const int USsensorRightEcho1 = 17;
//back
static const int USsensorBackTrigger1 = 20;
static const int USsensorBackEcho1 = 21;
int backDistance = 0;


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(USsensorFrontTrigger1, OUTPUT);
  pinMode(USsensorFrontEcho1, INPUT);
  pinMode(USsensorFrontTrigger2, OUTPUT);
  pinMode(USsensorFrontEcho2, INPUT);
  pinMode(USsensorLeftTrigger1, OUTPUT);
  pinMode(USsensorFrontEcho1, INPUT);
  pinMode(USsensorRightTrigger1, OUTPUT);
  pinMode(USsensorRightEcho1, INPUT);
  pinMode(USsensorBackTrigger1, OUTPUT);
  pinMode(USsensorBackEcho1, INPUT);

  mysmartservo.begin(9600);
  delay(5);
  mysmartservo.assignDevIdRequest();
  delay(50);
  loopTime = millis();
  pinMode(mosfet,OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  backDistance = measureDist(USsensorBackTrigger1, USsensorBackEcho1);  //x start
  leftDistance = measureDist(USsensorLeftTrigger1, USsensorLeftEcho1);  //y start
  Serial.println("back distance:");
  Serial.print(backDistance);
  Serial.println("left distance:");
  Serial.print(leftDistance);
  
  Serial.begin(9600);
}


void loop() {
  //Measure distance in the front
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    sscanf(data.c_str(), "%d;%d;%d;%d", &id_ard, &x_ard, &y_ard, &heading_ard);
  }

  int frontDistance = measureDist(USsensorFrontTrigger1, USsensorFrontEcho1);
  int frontDistance2 = measureDist(USsensorFrontTrigger2, USsensorFrontEcho2);
  if (frontDistance <15 || frontDistance2< 15){
    stilstand();
  }
  odometry(Position);
  Serial.println(Position.x_pos);
  SerialCommunication();
  //int *distanceF = measureDist(); //located in different file 
  if (x_ard > 0 && y_ard > 0) {
    movement();
  }
  grappingSample(); //servo movement
  Serial.println(x_pos);
}

// Timer 90 seconden
// Start indien niets gevonden wordt
// Wanneer iets gedropt is, niet direct opnieuw targeten met de camera!
