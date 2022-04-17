//remove the comments for better reading
/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */
 
// Deze
// 0, 1, 2, 3, 7  - micro/leonardo interrupt
// Y positie mogelijks niet helemaal correct. Dan forward offset aanpassen.
// Testen met motoraansturing.
/*

#include <Encoder.h>

//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

// White - Green
Encoder knobLeft(0, 1); //white/white - pink/green  
Encoder knobRight(2, 3); //white/white - orange/green
Encoder knobFront(6, 7); //yellow/white - purple/green

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
float heading;  //Wordt gebruikt op https://gm0.org/en/latest/docs/software/odometry.html?highlight=odometry
// In addition to the linear position, the robot has a heading  defined as the angle between the front of the robot and the global  axis. 
  
void loop() {
  odometry();
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
/*
  long newLeft, newRight, newFront, prev_positionLeft, prev_positionRight, prev_positionFronts;
  long delta_positionLeft, delta_positionRight, delta_positionFront;
  long phi, delta_positionMiddle, delta_positionLoodrecht;
  long delta_x, delta_y, x_pos, y_pos;
  
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  newFront = knobFront.read();
  if (newLeft != positionLeft || newRight != positionRight || newFront != positionFront) {
    
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.print(", Front = ");
    Serial.print(newFront);
    Serial.println();
    
    //New position  <-> get values from readings  https://gm0.org/en/latest/docs/software/odometry.html?highlight=odometry
    positionLeft = newLeft;   
    positionRight = newRight;
    positionFront = newFront;
    
    // My position calculation
    delta_positionLeft = positionLeft - prev_positionLeft;
    delta_positionRight = positionRight - prev_positionRight;
    delta_positionFront = positionFront - prev_positionFronts;
    
    phi = (delta_positionLeft - delta_positionRight) / trackwidth;
    delta_positionMiddle = (delta_positionLeft + delta_positionRight) / 2;
    delta_positionLoodrecht = delta_positionFront - (forward_offset * phi);
    
    delta_x = delta_positionMiddle * cos(heading) - delta_positionLoodrecht * sin(heading);
    delta_y = delta_positionMiddle * sin(heading) + delta_positionLoodrecht * cos(heading);
    
    x_pos += delta_x;
    y_pos += delta_y;
    heading += phi;
    
    Serial.print("X pos = ");
    Serial.print(x_pos);
    Serial.print(", Y pos = ");
    Serial.print(y_pos);
    Serial.print(", Heading = ");
    Serial.print(heading);
    Serial.print(phi);
    Serial.println();
    
    //Remember last position
    prev_positionLeft = positionLeft;
    prev_positionRight = positionRight;
    prev_positionFronts = positionFront; 
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobLeft.write(0);
    knobRight.write(0);
    knobFront.write(0);
    heading = 0;
  }
  */
/*
Nakijken of heading niet beteer is met een 'min last (huidig - last)'


https://www.youtube.com/watch?v=Av9ZMjS--gY

Deltaxr = R/2 * (DeltaSigma1 + DeltaSigma1)
Deltayr = R* ((x0/(2*y0))*(deltaSigma1-DeltaSigma2)+DeltaSigma3)
DeltaAngleR = (R/(2Y0)*(DeltaSigma2 - DeltaSigma1)
Alt: 

met R = wheel radius
DeltaSigma1, 2, 3 is denk ik de delta (dus last - huidig) van de gemeten data -  encoderticks
Deltaxr,yr, AngleR is denk ik huidig - last van x, y, agnle - movement of the robot relative to the last position (x0)
x0, y0 last/ huidige positie

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

---------------------------------

#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

//encoder
double XPos, YPos, Theta, XLeftPosDelta, XRightPosDelta, XPosDelta, YPosDelta, ThetaDelta, XPos0, YPos0, Theta0;

double position[] = {XPos, YPos, Theta};
double DeltaPosition[] = {XPosDelta, YPosDelta, ThetaDelta};
double Position0[] = {XPos0, YPos0, Theta0};

void Position(){
    // XLeftPosDelta, XRightPosDelta, YPosDelta from sensor(?) + delta moet huidige min vorige zijn
  DeltaPosition[2] =  (XLeftPosDelta - XRightPosDelta) / WIDTH_OFFSET;  //Theta delta, need it later on again so calculate first
  DeltaPosition[0] = (XLeftPosDelta + XRightPosDelta) / 2;  // Xposition Delta
  DeltaPosition[1] = YPosDelta - (DeltaPosition[2] * FORWARD_OFFSET);  //FORWARD_OFFSET is a correction

  for (int i = 0; i < ARRAY_SIZE(position); i++){
    position[i] = DeltaPosition[i] + Position0[i];    //Nog niet het correcte(?), nog euler shit toevoegen
  }

  return 0;

  
}
*/

