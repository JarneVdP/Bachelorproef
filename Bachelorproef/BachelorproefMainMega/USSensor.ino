int measureDist(int trigger, int echo){   //Ultrasone sensors plaatsen (2 vanvoor?), ook op de zijkanten en achter en indien deze een afstand dicht genoeg meten, rij achteruit.
  digitalWrite(trigger, LOW);
  delayMicroseconds(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  int duration = pulseIn(echo, HIGH);
  digitalWrite(echo, LOW);

  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return distance;
} 

void measureStartPos(){
  for (int i = 0; i < 10; i++) {
    backDistancearr += measureDist(USsensorBackTrigger1, USsensorBackEcho1);  //x start
    leftDistancearr += measureDist(USsensorLeftTrigger1, USsensorLeftEcho1);  //y start
  }
  Position.x_pos = (backDistancearr / 10  )+ 15;
  Position.y_pos = (leftDistancearr / 10  )+ 15;
}
