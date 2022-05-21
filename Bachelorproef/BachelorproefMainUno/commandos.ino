int stop_statement = 0;

float heading_station(double x, double y, double x_station, double y_station) //hoek berekenen
{
  float hoek = atan((y - y_station) / (x - x_station));
  //Serial.println("hoek");
  //Serial.print(hoek);
  return hoek;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void draai(int richting, float heading, float heading_station, double x, double x_station) //weten of hij rechts of links moet draaien
{

  unsigned long currentMillis = millis();
  float heading_backstation_0 = (hoek2 + heading_station  );
  float heading_backstation_1 = (-hoek2 + heading_station );
  if (richting == 0) //rechts
  {
    //Serial.println("statement");
    //Serial.println(statement);
    if (heading_station != 0 && statement == 0)// als hoek verschillend is van nul doe deze code
    {
      if (heading <= heading_backstation_0)
      {
        if (currentMillis - previousMillis >= interval)
        {
          //Serial.println("rechts_0");
          previousMillis = currentMillis;
          eigenasdraaienrechts();
        }
        if (heading > heading_backstation_0 - error_h) {
          statement = 1;
        }
        //Serial.println("BG2_0");
      }
      else if (heading >= heading_backstation_0)
      {
        //Serial.println("BG1_0");
        if (currentMillis - previousMillis >= interval)
        {
          //Serial.println("rechts_0");
          previousMillis = currentMillis;
          eigenasdraaienlinks();
        }
        if (heading < heading_backstation_0 + error_h ) {
          statement = 1;
        }
      }
      if (statement == 1) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          stilstand();
        }
      }
    }

    //x richting
    else if ( x > x_station + error_x  ) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        vooruit();
      }
      if (x <= x_station + error_x )
      {
        stop_statement = 1;
      }
    }
    else if (stop_statement == 1) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        stilstand();
      }
    }
  }
  else if (richting == 1)
  {
    if (heading_station != 0 && statement == 0)// als hoek verschillend is van nul doe deze code
    {
      if (heading >= heading_backstation_1)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienlinks();
        }
        if (heading < heading_backstation_1 + error_h ) {
          statement = 1;
        }
      }
      else if (heading <= heading_backstation_1)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienrechts();
        }
        if (heading > heading_backstation_1 - error_h) {
          statement = 1;
        }
      }
      if (statement == 1) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          stilstand();
        }
      }
    }
    //x richting

    else if ( x > x_station + error_x  ) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        vooruit();
      }
      if (x <= x_station + error_x )
      {
        stop_statement = 1;
      }
    }
    else if (stop_statement == 1) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        stilstand();
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void draai2(int richting, float heading, float heading_station, double x, double x_station) //weten of hij rechts of links moet draaien
{

  unsigned long currentMillis = millis();

  if (richting == 0) //rechts
  {
    if (heading_station != 0 && statement == 0)// als hoek verschillend is van nul doe deze code
    {
      if (heading <= hoek3 - heading_station)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienrechts();
        }
        if (heading > hoek3 - heading_station - error_h) {
          statement = 1;
        }
      }
      else if (heading >= hoek3 - heading_station)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienlinks();
        }
        if (heading < hoek3 - heading_station + error_h ) {
          statement = 1;
        }
      }
      if (statement == 1) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          stilstand();

        }
      }
    }

    //x richting

    else if ( x < x_station - error_x  ) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        vooruit();
      }
      if (x >= x_station - error_x )
      {
        stop_statement = 1;
      }
    }
    else if (stop_statement == 1) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        stilstand();
      }
    }
  }
  else if (richting == 1)
  {
    if (heading_station != 0 && statement == 0)// als hoek verschillend is van nul doe deze code
    {
      /*
      Serial.println("heading");
      Serial.println(heading);
      Serial.println("heading_station");
      Serial.println(heading_station);
      */
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
        }
      }
    }
    //x richting

    else if ( x < x_station - error_x  ) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        vooruit();
      }
      if (x >= x_station - error_x )
      {
        stop_statement = 1;
      }
    }
    else if (stop_statement == 1) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        stilstand();

      }
    }
  }
}
