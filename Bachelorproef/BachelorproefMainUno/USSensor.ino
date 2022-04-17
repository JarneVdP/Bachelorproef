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