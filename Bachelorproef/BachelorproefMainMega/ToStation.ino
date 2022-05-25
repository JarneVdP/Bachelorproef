void goStation(double x, double y, double x_station, double y_station, float heading, float heading_station) {
  unsigned long currentMillis = millis();
  if (( heading <= hoek && heading >= (hoek * -1)) && direction_bot == 0){
    direction_bot=1;
    }
  else if ((heading > hoek || heading < (hoek * -1)) && direction_bot == 0){
    direction_bot=2;
    }
  if ( direction_bot == 1) //check robot kijkt voor 
  {
    if (x_station > x && position_bot == 0 ) // sation voor robot
    {
      GoTo2( x, heading, x_station,  heading_station); //vooruit
      if (x_station < x + error_x )
      {
        position_bot = 1;
        direction_bott = 1;
      }
    }
    else if (x_station <= x && position_bot == 0) //achteruit, y robot groter
    {
      if ( y == y_station) // letterlijk achter de robot
      {
        draai_bot = 3;
      }
      if (y > y_station && draai_bot != 3)  // station links dus links draaien
      {
        draai_bot = 1;
      }
      else if (y < y_station && draai_bot != 3) // station rechts dus rechts draaien
      {
        draai_bot = 2;
      }
    }
    if ( heading > hoek || heading < (hoek * -1) )
    {
      direction_bot = 3; // loop af? ga naar volgende
    }
  }
  else if (direction_bot == 2) // robot ligt tussen hoek 90 en -90
  {
    if (x_station < x && position_bot == 0) //station ligt voor robot
    {
      GoTo3( x, heading, x_station,  hoek2 + heading_station); //vooruit

      if (x_station > x - error_x )
      {
        position_bot = 1;
        direction_bott = 1;
      }
    }
    else if ( x_station >= x && position_bot == 0)
    {
      if ( y == y_station) // letterlijk achter de robot
      {
        draai_bot = 4;
      }
      if (y > y_station && draai_bot != 4 ) // station links dus rechts draaien
      {
        draai_bot = 5;
      }
      else if (y < y_station && draai_bot != 4) // station rechts dus links draaien
      {
        draai_bot = 6;
      }
    }
    if ( heading <= hoek && heading >= (hoek * -1) )
    {
      direction_bot = 3; // loop af? ga naar volgende
    }
  }
  if (draai_bot == 1 && direction_bott == 0)
  {
    draai(1, heading, heading_station, x, x_station);
    if (x <= x_station)
    {
      direction_bott = 1;
    }
  }
  if (draai_bot == 2 && direction_bott == 0)
  {
    draai(0, heading, heading_station, x, x_station );
    if (x <= x_station)
    {
      direction_bott = 1;
    }
  }
  if (draai_bot == 3 && direction_bott == 0)
  {
    draai(0, heading, 0.01, x, x_station );
    if (x <= x_station)
    {
      direction_bott = 1;
    }
  }
  if (draai_bot == 4 && direction_bott == 0)
  {
    draai2(1, heading, 0.01 , x, x_station );
    if (x >= x_station)
    {
      direction_bott = 1;
    }
  }
  if (draai_bot == 5 && direction_bott == 0)
  {

    draai2(1, heading, heading_station, x, x_station);
    if (x >= x_station)
    {
      direction_bott = 1;
    }
  }
  if (draai_bot == 6 && direction_bott == 0)
  {
    draai2(1, heading, heading_station, x, x_station);
    if (x >= x_station)
    {
      direction_bott = 1;
    }
  }
  else if (direction_bott == 1) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      stilstand();
    }
  }
}
