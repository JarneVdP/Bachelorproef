float doel_heading;
double doel_x;

void GoTo(double x, float heading, double camera_x, float camera_heading) {
  unsigned long currentMillis = millis();
  camera_heading = camera_heading * 3.1415926535 / 180;
  //doel_heading = heading + camera_heading;
  //doel_x= camera_y + x;
  //hoek
  if (camera_heading != 0 && statement == 0)
  {
    if (heading >= camera_heading )
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienlinks();
      }
      if (heading < camera_heading + error_h) {
        statement = 1;
      }
    }
    else if (heading <= camera_heading  )
    {

      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienrechts();
      }
      if (heading > camera_heading - error_h) {
        statement = 1;
      }
    }
    if (statement == 1) {
      //delay(5000);
      if (currentMillis - previousMillis >= interval) {
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

  //x richting
  else if ( x < camera_x - error_x  ) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      vooruit();
    }
  }
  else {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      stilstand();
    }
  }
}
