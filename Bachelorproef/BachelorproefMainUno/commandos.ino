int stop_statement = 0;

float heading_station(double x, double y, double x_station, double y_station) // hoek berekenen
{
  float hoek = atan((y - x_station) / (x - y_station)) * -1;
  return hoek;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void draai(int richting, float heading, float heading_station, double x, double y_station) // weten of hij rechts of links moet draaien, in gostation als robot tussen -90 en +90 en station ligt achter hem
{
  unsigned long currentMillis = millis();
  float heading_backstation_0 = (hoek2 + heading_station);
  float heading_backstation_1 = (-hoek2 + heading_station);
  if (richting == 0) // links
  {
    if (heading_station != 0 && statement == 0) // als hoek verschillend is van nul doe deze code
    {
      if (heading <= heading_backstation_0)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienlinks();
        }
        if (heading > heading_backstation_0 - error_h)
        {
          statement = 1;
        }
      }
      else if (heading >= heading_backstation_0)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienrechts();
        }
        if (heading < heading_backstation_0 - error_h)
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
    }

    // x richting
    else if (x > y_station - error_x)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        vooruit();
      }
      if (x <= y_station - error_x)
      {
        stop_statement = 1;
      }
    }
    else if (stop_statement == 1)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        stilstand();
      }
    }
  }
  else if (richting == 1)
  {
    if (heading_station != 0 && statement == 0) // als hoek verschillend is van nul doe deze code
    {
      if (heading >= heading_backstation_1)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienrechts();
        }
        if (heading < heading_backstation_1 - error_h)
        {
          statement = 1;
        }
      }
      else if (heading <= heading_backstation_1)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          eigenasdraaienlinks();
        }
        if (heading > heading_backstation_1 - error_h)
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
    }
    // x richting

    else if (x > y_station - error_x)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        vooruit();
      }
      if (x <= y_station - error_x)
      {
        stop_statement = 1;
      }
    }
    else if (stop_statement == 1)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        stilstand();
      }
    }
    /*
    Serial.println("x ");
    Serial.println(x);
    Serial.println("y_station ");
    Serial.println(y_station);
    Serial.println("stop_statement ");
    Serial.println(stop_statement);
    */
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void draai2(int richting, float heading, float heading_station, double x, double y_station) // weten of hij rechts of links moet draaien, in gostation als robot kleiner dab -90 en groter dan +90 en station ligt achter hem
{
  {
    unsigned long currentMillis = millis();
    /*
    Serial.println("richting  ");
    Serial.println(richting);
    */
    if (richting == 0) // links
    {
      /*
      Serial.println("x ");
      Serial.println(x);
      Serial.println("y_station ");
      Serial.println(y_station);
      Serial.println(statement);
      */
      if (heading_station != 0 && statement == 0) // als hoek verschillend is van nul doe deze code
      {
        /*
        Serial.println("heading");
        Serial.println(heading);
        Serial.println("heading_station");
        Serial.println(heading_station);
        */
        if (heading <= hoek3 + heading_station)
        {
          if (currentMillis - previousMillis >= interval)
          {
            previousMillis = currentMillis;
            eigenasdraaienlinks();
          }
          if (heading > hoek3 + heading_station - error_h)
          {
            statement = 1;
          }
        }
        else if (heading >= hoek3 + heading_station)
        {
          if (currentMillis - previousMillis >= interval)
          {
            previousMillis = currentMillis;
            eigenasdraaienrechts();
          }
          if (heading < hoek3 + heading_station - error_h)
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
      }

      // x richting

      else if (x < y_station - error_x)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          vooruit();
        }
        if (x >= y_station - error_x)
        {
          stop_statement = 1;
        }
      }
      else if (stop_statement == 1)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          stilstand();
        }
      }
    }
    else if (richting == 1)
    {
      if (heading_station != 0 && statement == 0) // als hoek verschillend is van nul doe deze code
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
      }
      // x richting

      else if (x < y_station - error_x)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          vooruit();
        }
        if (x >= y_station - error_x)
        {
          stop_statement = 1;
        }
      }
      else if (stop_statement == 1)
      {
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis;
          stilstand();
        }
      }
    }
  }
}