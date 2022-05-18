#include <Encoder.h>

//#########
//#Odometry#
//##########
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
/*
Encoder knobLeft(0, 1); //white/white - pink/green  
Encoder knobRight(2, 3); //white/white - orange/green
Encoder knobFront(6, 7); //yellow/white - purple/green
*/
//Mega
Encoder knobRight(2, 3); //white/white - pink/green
Encoder knobLeft(18, 19); //white/white - orange/green
Encoder knobFront(20, 21); //yellow/white - purple/green


int positionLeft  = 1;
int positionRight = 1;
int positionFront = 1;

double newLeft, newRight, newFront, prev_positionLeft, prev_positionRight, prev_positionFront;
double delta_positionLeft, delta_positionRight, delta_positionFront;
float theta, deltatheta = 0;
double dx, dy;
//double delta_x, delta_y, x_pos, y_pos;

int forward_offset = 15;//20; //cm
int trackwidth = 28; //cm, breedte tussen 2 wielen 
int wheelradius = 3; //cm , straal
int ticksPsec = 1600; //ticks per second
double cmPerTick = 2.0 * 3.14159265 * wheelradius / ticksPsec;
//float heading;  //Wordt gebruikt op https://gm0.org/en/latest/docs/software/odometry.html?highlight=odometry  hoek van de robot



void odometry(PositionStruct &Position) {
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
  if (Position.heading > (-90* 3.14159265 / 180) && Position.heading < (90* 3.14159265 / 180)) {
    Position.x_pos += dx * cos(Position.heading) - dy * sin(Position.heading); //sin and cos in radians
    Position.y_pos += -1 * (dx * sin(Position.heading) + dy * cos(Position.heading));
  }
  else if (Position.heading < (-90* 3.14159265 / 180) || Position.heading > (90* 3.14159265 / 180)) {
    Position.x_pos -= (-1)*dx * cos(Position.heading) - (-1)* dy * sin(Position.heading); //sin and cos in radians
    Position.y_pos -= -1* (dx * sin(Position.heading) + dy * cos(Position.heading));
  } 
  //heading += heading + (deltatheta/float(2));
  Position.heading += -1 *deltatheta;
  if (Position.heading < -6.2831853072){  //6.2831853072 = 360 degrees
    Position.heading += 6.2831853072;}
    
  if (Position.heading > 6.2831853072){  //6.2831853072 = 360 degrees
    Position.heading -= 6.2831853072;}

  //print current position
  if(newLeft != prev_positionLeft || newRight != prev_positionRight || newFront != prev_positionFront){
    Serial.print("X pos = ");
    Serial.print(Position.x_pos);
    Serial.print(", Y pos = ");
    Serial.print(Position.y_pos);
    Serial.print(", Heading = ");
    Serial.print(Position.heading); Serial.print("   ");
    Serial.println(Position.heading * 180/ 3.14159265358979);
  }

  //Old position
  prev_positionLeft = newLeft;
  prev_positionRight = newRight;
  prev_positionFront = newFront; 

  //reset
  /*if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobLeft.write(0);
    knobRight.write(0);
    knobFront.write(0);
    Position.heading = 0;
  }*/
}
