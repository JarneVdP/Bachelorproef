#ifndef Encoder_h
#define Encoder_h

//Arduino Pins, pinnen nog niet getest, nog niet vastgelegd
//pinout encoders
static const int encoderFront = 3;
static const int encoderLeft  = 2;
static const int encoderRight = 1;

//Encoder
extern double XPos, YPos, Theta, XLeftPosDelta, XRightPosDelta, XPosDelta, YPosDelta, ThetaDelta, XPos0, YPos0, Theta0;

extern double position[];// = {XPos, YPos, Theta};
extern double DeltaPosition[];// = {XPosDelta, YPosDelta, ThetaDelta};
extern double Position0[];// = {XPos0, YPos0, Theta0};

extern int XPoscenter;// = XPos + 0;
extern int YPoscenter;// = YPos + 0; 


static const int TICKS_PER_REV = 400;  //waarschijnlijk 400 of 1600
static const int WHEEL_RADIUS = 30; // mm 60mm/2
static const int GEAR_RATIO = 1; // output (wheel) speed / input (encoder) speed
static const int WIDTH_OFFSET = 10; // mm; distance between the left and right wheels. Wielen in de rijrichting geplaatst
static const int FORWARD_OFFSET = 4; // mm; offset of the lateral wheel. Wiel loodrecht op de rijrichting.

void Position();

#endif