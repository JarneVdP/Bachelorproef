// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
#include <Encoder.h>

//#########
//#Odometry#
//##########
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

Encoder knobLeft(0, 1); //white/white - pink/green  
Encoder knobRight(2, 3); //white/white - orange/green
Encoder knobFront(6, 7); //yellow/white - purple/green


int positionLeft  = 1;
int positionRight = 1;
int positionFront = 1;

double newLeft, newRight, newFront, prev_positionLeft, prev_positionRight, prev_positionFront;
double delta_positionLeft, delta_positionRight, delta_positionFront;
float theta, deltatheta = 0;
double dx, dy;
double delta_x, delta_y, x_pos, y_pos;

int forward_offset = 15;//20; //cm
int trackwidth = 28; //cm, breedte tussen 2 wielen 
int wheelradius = 3; //cm , straal
int ticksPsec = 1600; //ticks per second
double cmPerTick = 2.0 * 3.14159265 * wheelradius / ticksPsec;
float heading;  //Wordt gebruikt op https://gm0.org/en/latest/docs/software/odometry.html?highlight=odometry  hoek van de robot

//########
//#Serial#
//#########
int x_ard, y_ard, heading_ard, id_ard;

//###########
//#US Sensor#
//###########
//Pinnen voor ultrasonic sensor
static const int URsensorFrontTrigger1 = 4;
static const int URsensorFrontEcho1 = 5;
static const int URsensorFrontTrigger2 = 8;
static const int URsensorFrontEcho2 = 9;

//Ultrasone sensor
extern int *distanceF[2];


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

void movement() {
}


void SerialCommunication() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int str_len = data.length() + 1;
    char char_array[str_len];
    data.toCharArray(char_array, str_len);
    sscanf(data.c_str(), "%d;%d;%d;%d", &id_ard, &x_ard, &y_ard, &heading_ard);
    
    Serial.print("You sent me: ");
    Serial.print(data);
    Serial.print(" : ");
    Serial.print(id_ard);
    Serial.print(" & ");
    Serial.print(x_ard);
    Serial.print(" & ");
    Serial.print(y_ard);
    Serial.print(" & ");
    Serial.println(heading_ard);
  }
}


int *measureDist(){   //Ultrasone sensors plaatsen (2 vanvoor?), ook op de zijkanten en achter en indien deze een afstand dicht genoeg meten, rij achteruit.
  digitalWrite(URsensorFrontTrigger1, LOW);
  delayMicroseconds(1);
  digitalWrite(URsensorFrontTrigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(URsensorFrontTrigger1, LOW);
  int durationF1 = pulseIn(URsensorFrontEcho1, HIGH);
  digitalWrite(URsensorFrontEcho1, LOW);

  digitalWrite(URsensorFrontTrigger2, LOW);
  delayMicroseconds(1);
  digitalWrite(URsensorFrontTrigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(URsensorFrontTrigger2, LOW);
  int durationF2 = pulseIn(URsensorFrontEcho2, HIGH);
  digitalWrite(URsensorFrontEcho2, LOW);

  int distanceF1 = durationF1 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  int distanceF2 = durationF2 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //int distanceF[] = {distanceF1, distanceF2};

  // Displays the distance on the Serial Monitor
  Serial.print("Distance Forward: ");
  Serial.print(distanceF1, distanceF2);
  Serial.print("\t ");
  
  return 0;
} 


void odometry(){
  //currentposition based on encoder readings
  newLeft = -1 * knobLeft.read();
  newRight = -1* knobRight.read();
  newFront = knobFront.read();

  //print encoder rotations
    if(newLeft != prev_positionLeft || newRight != prev_positionRight || newFront != prev_positionFront){
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.print(", Front = ");
    Serial.print(newFront);
    Serial.println();
  }

  //delta position
  delta_positionLeft = newLeft - prev_positionLeft;
  delta_positionRight = newRight - prev_positionRight;
  delta_positionFront = newFront - prev_positionFront;

  //delta theta (hoek), delta x, delta y (hoek, positie, positie)
  deltatheta = cmPerTick * (delta_positionRight - delta_positionLeft) / trackwidth;
  dx = cmPerTick * (delta_positionLeft + delta_positionRight) / 2.0;
  dy = cmPerTick * (delta_positionFront - forward_offset * (delta_positionRight - delta_positionLeft) / trackwidth);

  //new position
  x_pos += dx * cos(theta) - dy * sin(theta); //sin and cos in radians
  y_pos += dx * sin(theta) + dy * cos(theta);
  //heading += heading + (deltatheta/float(2));
  heading += deltatheta;
  if (heading < -6.2831853072){  //6.2831853072 = 360 degrees
    heading += 6.2831853072;}
    
  if (heading > 6.2831853072){  //6.2831853072 = 360 degrees
    heading -= 6.2831853072;}

  //print current position
  if(newLeft != prev_positionLeft || newRight != prev_positionRight || newFront != prev_positionFront){
    Serial.print("X pos = ");
    Serial.print(x_pos);
    Serial.print(", Y pos = ");
    Serial.print(y_pos);
    Serial.print(", Heading = ");
    Serial.print(heading); Serial.print("   ");
    Serial.print(heading * 180/ 3.14159265358979);
    Serial.println();
  }

  //Old position
  prev_positionLeft = newLeft;
  prev_positionRight = newRight;
  prev_positionFront = newFront; 

  //reset
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobLeft.write(0);
    knobRight.write(0);
    knobFront.write(0);
    heading = 0;
  }
}
