// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
#include <Encoder.h>

//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
/*
// White - Green
Encoder knobLeft(0, 1); //white/white - pink/green  
Encoder knobRight(2, 3); //white/white - orange/green
Encoder knobFront(6, 7); //yellow/white - purple/green
*/

Encoder knobRight(2, 3); //white/white - pink/green
Encoder knobLeft(18, 19); //white/white - orange/green
Encoder knobFront(20, 21); //yellow/white - purple/green

void setup() {
  Serial.begin(9600);
  Serial.println("TwoKnobs Encoder Test:");
}

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

void loop() {
  odometry();
}

void odometry(){
  //currentposition based on encoder readings
  newLeft =  knobLeft.read();
  newRight = -1* knobRight.read();
  newFront = -1* knobFront.read();

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

/*
https://www.youtube.com/watch?v=Av9ZMjS--gY

encoder - robot - field

angles:
L = theta*r = alpha/360 * 2pi * r
theta in radians
R = 6cm
N = 1600 ticks per revolution
deltaN = huidige ticks, given by the software
s = afstand travelled
N1 = encoderLinks
N2 = encoderRechts
N3 = encodervooraan
B = offset naar wielvooraan

C = 2piR / N [cm/tick]
s = 2piR * deltaN / N <=> s = C* deltaN

deltaX  =C * ((DeltaN1 + deltaN2)/2)  
deltaY = c* (deltaN3 - B*((DeltaN2 - DeltaN1)/L))
DeltaTheta = C* ((DeltaN1 - deltaN2)/2)

Naar field coördinates:
x = x0 + deltaX* cos(theta) - deltaY * sinTheta
y = y0 + deltaX* sin(theta) - deltaY * cosTheta
Theta = Theta0 + deltaTheta //shoould be small

positie kan in een tuple of some sort bijgeohouden worden

*/
