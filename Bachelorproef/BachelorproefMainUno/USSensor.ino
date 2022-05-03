int measureDist(int trigger, int echo){   //Ultrasone sensors plaatsen (2 vanvoor?), ook op de zijkanten en achter en indien deze een afstand dicht genoeg meten, rij achteruit.
  digitalWrite(trigger, LOW);
  delayMicroseconds(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  int duration = pulseIn(echo, HIGH);
  digitalWrite(echo, LOW);

  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  // Displays the distance on the Serial Monitor
  Serial.print("Distance Forward: ");
  Serial.print(distance);
  return distance;
} 
