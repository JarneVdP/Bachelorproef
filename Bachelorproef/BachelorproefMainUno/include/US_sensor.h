#ifndef US_sensor_h
#define US_sensor_h

//pinout US sensor
static const int URsensorFrontTrigger1 = 4;
static const int URsensorFrontEcho1 = 5;
static const int URsensorFrontTrigger2 = 6;
static const int URsensorFrontEcho2 = 7;

//Ultrasone sensor
extern int *distanceF[2];

int *measureDist();

#endif