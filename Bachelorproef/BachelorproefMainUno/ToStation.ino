double hoek = 90 * (3.14159265359 / 180);
double hoek2 = 180 * (3.14159265359 / 180);
double hoek3 = 360 * (3.14159265359 / 180);
float error_h = 0.4 * (3.14159265359 / 180);

unsigned long previousMillis = 1;
const long interval = 100;
double error_x = 10;
int direction_bot = 0;
int position_bot = 0;
int direction_bott = 0;
int draai_bot = 0;

void goTo(double x, double y, double x_station, double y_station, float heading, float heading_station)
{
  unsigned long currentMillis = millis();
  // kijken hoe we gericht staan
  if (heading <= hoek && heading >= (hoek * -1) && direction_bot == 0)
  {
    direction_bot = 1; // vooruit kijken
  }
  else if (heading > hoek || heading < (hoek * -1) && direction_bot == 0)
  {
    direction_bot = 2; // achteruit kijken
  }

  if (direction_bot == 1) // kijkt vooruit
  {
    if (y_station > x && position_bot == 0) // station checken,station is voor robot
    {
      mapp2(x, heading, y_station, heading_station); // vooruit naar station rijden: hoek te draaien is tussen 90 en -90
      if (y_station < x + error_x)                   // om uit de loop te komen
      {
        position_bot = 1;   // niet meer in volgende ifs , doel is bereikt
        direction_bott = 1; // niet meer in volgende ifs , doel is bereikt
      }
    }
    else if (y_station <= x && position_bot == 0) // achter de robot. Hoek te draaien groter dan -90 of +90
    {
      if (y == x_station) // letterlijk achter de robot
      {
        draai_bot = 3; //
      }
      if (y > x_station && draai_bot != 3) // station links dus links draaien
      {
        draai_bot = 1;
      }
      else if (y < x_station && draai_bot != 3) // station rechts dus rechts draaien
      {
        draai_bot = 2;
      }
    }
    if (heading > hoek || heading < (hoek * -1))
    {
      direction_bot = 3; // loop af? ga naar volgende
    }
  }
  else if (direction_bot == 2) // robot kijkt achteruit
  {
    if (y_station < x && position_bot == 0) // station ligt voor robot
    {
      mapp3(x, heading, y_station, hoek2 + heading_station); // vooruit

      if (y_station > x - error_x)
      {
        position_bot = 1;
        direction_bott = 1;
      }
    }
    else if (y_station >= x && position_bot == 0)
    {
      if (y == x_station) // letterlijk achter de robot
      {
        draai_bot = 4;
      }
      if (y > x_station && draai_bot != 4) // station links dus rechts draaien
      {
        draai_bot = 5;
      }
      else if (y < x_station && draai_bot != 4) // station rechts dus links draaien
      {
        draai_bot = 6;
      }
    }
    if (heading <= hoek && heading >= (hoek * -1))
    {
      direction_bot = 3; // loop af? ga naar volgende
    }
  }
  if (draai_bot == 1 && direction_bott == 0) // draai naar links
  {
    draai(0, heading, heading_station, x, y_station); // 0 is links
    if (x <= y_station)
    {
      direction_bott = 1; // doel bereikt, ga eruit
    }
  }
  if (draai_bot == 2 && direction_bott == 0)
  {
    draai(1, heading, heading_station, x, y_station); // 1 is rechts
    if (x <= y_station)
    {
      direction_bott = 1; // doel bereikt, ga eruit
    }
  }
  if (draai_bot == 3 && direction_bott == 0) // achter de robot
  {
    draai(0, heading, -0.01, x, y_station); // via links draaien
    /*
    Serial.print("heading ");
    Serial.print(heading);
    Serial.print(", headingstation ");
    Serial.println(heading_station);
    */
    if (x <= y_station)
    {
      direction_bott = 1; // doel bereikt, ga eruit
    }
  }
  if (draai_bot == 4 && direction_bott == 0)
  {
    draai2(1, heading, 0.01, x, y_station);
    if (x >= y_station)
    {
      direction_bott = 1;
    }
  }
  if (draai_bot == 5 && direction_bott == 0)
  {

    draai2(1, heading, heading_station, x, y_station);
    if (x >= y_station)
    {
      direction_bott = 1;
    }
  }
  if (draai_bot == 6 && direction_bott == 0)
  {
    draai2(1, heading, heading_station, x, y_station);
    if (x >= y_station)
    {
      direction_bott = 1;
    }
  }
  else if (direction_bott == 1)
  {
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      stilstand();
    }
  }
}