void grappingSample(){
  //MotorGoUp(10); // bring glider to right vertical position
  //delay(5000);
  servo(-30,9.3,-104.6);
  digitalWrite(mosfet,HIGH);
  delay(500);
  servo(90,90,0);
}

void grappingSample17(){
  //MotorGoUp(10); // bring glider to right vertical position
  //delay(5000);
  servo(-30,9.3,-104.6);
  digitalWrite(mosfet,HIGH);
  delay(500);
  servo(-30,70,-104.6);
  servo(-45,-35,130);
  servo(90,90,0);
}

void placeDownSample(){
  //MotorGoDown(10); //bring glider to original position
  //delay(5000);
  servo(-30,9.3,-104.6);
  digitalWrite(mosfet,LOW);
  delay(500);
}

void placeDownSample180(){
  //MotorGoDown(10); //bring glider to original position
  //delay(5000);
  servo(-30,-45,90);
  digitalWrite(mosfet,LOW);
  delay(500);
}

void MotorGoUp(int steps)
{
    digitalWrite(dirPin, HIGH);
    for(int i = 0; i<steps; i++)
    {
      digitalWrite(stepPin, HIGH);
      delay(mtr_speed);
      digitalWrite(stepPin, LOW);
      delay(mtr_speed);
    } 
}

void MotorGoDown(int steps)
{
    digitalWrite(dirPin, LOW);
    for(int i = 0; i<steps; i++)
    {
      digitalWrite(stepPin, HIGH);
      delay(mtr_speed);
      digitalWrite(stepPin, LOW);
      delay(mtr_speed);
    } 
}

void servo(int A,int B, int C){
  mysmartservo.moveTo(1,A-correctServ1,50);
    mysmartservo.moveTo(2,B-correctServ2,50);   //device ID, angle, speed;  absolute angle move;
    mysmartservo.moveTo(3,C-correctServ3,50);
    loopTime = millis();
    while(millis() - loopTime < 2000){} // bring arm to sample
}