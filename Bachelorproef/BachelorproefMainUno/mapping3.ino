void mapp3(double x, float heading, double y_station, float heading_station)
{ // heading station, van positie naar station rijden. kleiner dan -90en groter dan +90
  unsigned long currentMillis = millis();
  // doel_x= camera_x + x;
  // hoek
  if (heading_station != 0 && statement == 0) // als hoek verschillend is van nul doe deze code
  {
    if (heading >= heading_station)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienrechts();
      }
      if (heading < heading_station - error_h)
      {
        statement = 1;
      }
    }

    else if (heading <= heading_station)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienlinks();
      }
      if (heading > heading_station - error_h)
      {
        statement = 1;
      }
    }
    if (statement == 1)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        stilstand();
      }
    }
    /*
    Serial.print("heading ");
    Serial.print(heading);
    Serial.print(", heading_station ");
    Serial.println(heading_station);
    */
  }

  // x richting

  else if (x > y_station - error_x)
  {
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      vooruit();
      /*
      Serial.print("x ");
      Serial.print(x);
      Serial.print(", y_station");
      Serial.println(y_station);
      */
    }
  }
  else
  {
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      stilstand();
    }
  }
}