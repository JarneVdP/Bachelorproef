void GoTo2(double x, float heading, double x_station, float heading_station) {
  Serial.println(" aan het draaien");
  Serial.print(statement);
  unsigned long currentMillis = millis();
  //doel_x= camera_x + x;
  //hoek
  if ( statement == 0)// als hoek verschillend is van nul doe deze code
  {
    Serial.println("in de functie");
    if (heading >= heading_station)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienlinks();
      }
      if (heading < heading_station + error_h ) {
        statement = 1;
      }
    }
    else if (heading <= heading_station)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        eigenasdraaienrechts();
      }
      if (heading > heading_station - error_h) {
        statement = 1;
      }
    }
    if (statement == 1) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        stilstand();
        statement = 0;
      }
    }
  }

  //x richting
  else if ( x < x_station - error_x  ) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      vooruit();
    }
  }
  else {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      stilstand();
      statement = 0;
    }
  }
  /*
    Serial.println("x");
    Serial.println(x);
    Serial.println("x_station");
    Serial.println(x_station);
  */
}
