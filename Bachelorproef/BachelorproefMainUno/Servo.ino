void action() {
    servo(90,0,-45);
    
    delay(1000);
    
    digitalWrite(mosfet,HIGH);
    
    delay(1000);

    servo(0,45,0);
    
    delay(500);
    
    digitalWrite(mosfet,LOW);
    
    delay(1000);
    
//    digitalWrite(mosfet,HIGH);
//    delay(1000);
//    digitalWrite(mosfet,LOW);
//    delay(1000);
//    MotorGoUp(100);
//    delay(1000);
//    mysmartservo.moveTo(1,90-correctServ1,50);
//    mysmartservo.moveTo(2,0-correctServ2,50);   //device ID, angle, speed;  absolute angle move;
//    mysmartservo.moveTo(3,-45-correctServ3,50);
//    loopTime = millis();
//    while(millis() - loopTime < 3000){} // bring arm back to original position
//    delay(1000);
//    digitalWrite(mosfet,HIGH); //zuigsysteem aan
//    delay(1000);
//    MotorGoDown(100);
//    delay(1000);
//    mysmartservo.moveTo(1,0-correctServ1,50);
//    mysmartservo.moveTo(2,45-correctServ2,50);   //device ID, angle, speed;  absolute angle move;
//    mysmartservo.moveTo(3,0-correctServ3,50);
//    loopTime = millis();
//    while(millis() - loopTime < 3000){} // bring arm back to original position
//    delay(1000);
//    digitalWrite(mosfet,LOW);
//    delay(1000);
}

void grapping(){
  MotorGoUp(10); // bring glider to right vertical position
  delay(5000);
  mysmartservo.moveTo(1,90-correctServ1,50);
  mysmartservo.moveTo(2,0-correctServ2,50);   // device ID, angle, speed;  absolute angle move;
  mysmartservo.moveTo(3,-45-correctServ3,50);
  loopTime = millis();
  while(millis() - loopTime < 3000){} // bring suction cup to sample
}

void original(){
  MotorGoDown(10); //bring glider to original position
  delay(5000);  
  mysmartservo.moveTo(1,0-correctServ1,50);
  mysmartservo.moveTo(2,45-correctServ2,50);   //device ID, angle, speed;  absolute angle move;
  mysmartservo.moveTo(3,0-correctServ3,50);
  loopTime = millis();
  while(millis() - loopTime < 3000){} // bring arm back to original position
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
    while(millis() - loopTime < 3000){} // bring arm to sample
}
