float doel_heading;
double doel_x;

void mapp(double x, float heading, double camera_y, float camera_heading)
{ // naar sample rijden
  unsigned long currentMillis = millis();
  camera_heading = camera_heading * 3.1415926535 / 180;
  // doel_heading = heading + camera_heading; //cameraheading naar doelheading veranderen in de functie
  // doel_x= camera_y + x;   //aanzetten camera_y naar doel_x omzetten
  // hoek
  if (camera_heading != 0 && statement == 0) // check if sample is niet voor de robot
  {
    if (heading >= camera_heading) // rechts draaien
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienrechts();
      }
      if (heading < camera_heading + error_h)
      { // doel bereikt
        statement = 1;
      }
    }
    else if (heading <= camera_heading) // links draaien
    {

      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienlinks();
      }
      if (heading > camera_heading - error_h)
      { // doel bereikt
        statement = 1;
      }
    }
    if (statement == 1)
    { // stilstand , zonder werkte het niet
      // delay(5000);
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;

        stilstand();
      }
    }
    /*
    Serial.println("camera_heading");
    Serial.println(camera_heading);
    Serial.println("heading");
    Serial.println(heading);
    Serial.println("statement");
    Serial.println(statement);
    */
  }

  // x richting
  else if (x < camera_y - error_x)
  { // hoek is in orde, rij vooruit naar sample
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      vooruit();
    }
  }
  else
  {
    if (currentMillis - previousMillis >= interval)
    { // stilstand
      previousMillis = currentMillis;
      stilstand();
    }
  }
}