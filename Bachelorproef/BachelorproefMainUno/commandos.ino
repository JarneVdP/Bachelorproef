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
  Serial.println("heading_backstation_0  ");
  Serial.println(heading_backstation_0);
  Serial.println("heading_backstation_1  ");
  Serial.println(heading_backstation_1);
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
          Serial.println("stil_0");
        }
      }
    }

    //x richting
    else if ( x > x_station + error_x  ) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        vooruit();
        Serial.println("BG3_0");
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
        Serial.println("BG4_0");
      }
    }
  }
  else if (richting == 1)
  {
    Serial.println("statement_1 ");
    Serial.println(statement);
    if (heading_station != 0 && statement == 0)// als hoek verschillend is van nul doe deze code
    {
      if (heading >= heading_backstation_1)
      {
        Serial.println("BG1_1");
        if (currentMillis - previousMillis >= interval)
        {
          Serial.println("links_1");
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
          Serial.println("links_1");
          previousMillis = currentMillis;
          eigenasdraaienrechts();
        }
        if (heading > heading_backstation_1 - error_h) {
          statement = 1;
        }
        Serial.println("BG2_1");
      }
      if (statement == 1) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          stilstand();
          Serial.println("stil_1");
        }
      }
    }
    //x richting

    else if ( x > x_station + error_x  ) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        vooruit();
        Serial.println("BG3_1");
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
        Serial.println("stil_1");
      }
    }
    Serial.println("x ");
    Serial.println(x);
    Serial.println("x_station ");
    Serial.println(x_station);
    Serial.println("stop_statement ");
    Serial.println(stop_statement);
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
      
      if (heading <= heading_station)
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
      else if (heading >= heading_station)
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
      if (statement == 1) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          stilstand();
        }
        stop_statement = 1;
      }
    }

   if (stop_statement == 1) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        stilstand();
      }
    }
  }
  else if (richting == 1)
  {

    Serial.println(statement);
    if (heading_station != 0 && statement == 0)// als hoek verschillend is van nul doe deze code
    {
      Serial.println("heading");
      Serial.println(heading);
      Serial.println("heading_station");
      Serial.println(heading_station);
      if (heading >= heading_station)
      {
        Serial.println("LINKS");
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
        Serial.println("RECHTS");
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
          Serial.println("stillll");
        }
      }
    }
    //x richting

    else if ( x < x_station - error_x  ) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        vooruit();
        Serial.println("voorrr");
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
