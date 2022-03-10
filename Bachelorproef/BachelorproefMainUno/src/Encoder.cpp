#include <Arduino.h>
#include "Encoder.h"

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
}


/*
void position(){
  int wheel0Pos, wheel1Pos, wheel2Pos, wheel3Pos;
  //Compute change in encoder positions
  int delt_m0 = wheel0Pos - lastM0;
  int delt_m1 = wheel1Pos - lastM1;
  int delt_m2 = wheel2Pos - lastM2;
  int delt_m3 = wheel3Pos - lastM3;


  //Compute displacements for each wheel
  displ_m0 = delt_m0 * wheelDisplacePerEncoderCount;
  displ_m1 = delt_m1 * wheelDisplacePerEncoderCount;
  displ_m2 = delt_m2 * wheelDisplacePerEncoderCount;
  displ_m3 = delt_m3 * wheelDisplacePerEncoderCount;


  //Compute the average displacement in order to untangle rotation from displacement
  displ_average = (displ_m0 + displ_m1 + displ_m2 + displ_m3) / 4.0;


  //Compute the component of the wheel displacements that yield robot displacement
  dev_m0 = displ_m0 - displ_average;
  dev_m1 = displ_m1 - displ_average;
  dev_m2 = displ_m2 - displ_average;
  dev_m3 = displ_m3 - displ_average;


  //Compute the displacement of the holonomic drive, in robot reference frame
  int delt_Xr = (dev_m0 + dev_m1 - dev_m2 - dev_m3) / twoSqrtTwo; 
  int delt_Yr = (dev_m0 - dev_m1 - dev_m2 + dev_m3) / twoSqrtTwo; 


  //Move this holonomic displacement from robot to field frame of reference
  robotTheta = IMU_ThetaRAD;    //geen IMU
  sinTheta = sin(robotTheta);
  cosTheta = cos(robotTheta);
  delt_Xf = delt_Xr * cosTheta - delt_Yr * sinTheta;
  delt_Yf = delt_Yr * cosTheta + delt_Xr * sinTheta;


  //Update the position
  XPos = lastX + delt_Xf;
  YPos = lastY + delt_Yf;
  Theta = robotTheta;
  lastM0 = wheel0Pos;
  lastM1 = wheel1Pos;
  lastM2 = wheel2Pos;
  lastM3 = wheel3Pos;
}
*/