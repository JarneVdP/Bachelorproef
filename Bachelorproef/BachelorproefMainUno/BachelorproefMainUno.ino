//Motor A
int enA = 10; 
int in1 = 9; //9
int in2 = 7; //7
//Motor B
int enB = 8; //8
int in3 = 5; 
int in4 = 4; 
/* debugger leds
int led1 = 6;
int led2 = 7;
int led3 = 8;
int led4 = 9;
*/
//###########
//#US Sensor#
//###########
//Pinnen voor ultrasonic sensor
//front

static const int USsensorFrontTrigger1 = 34; //rechts
static const int USsensorFrontEcho1 = 32;
static const int USsensorFrontTrigger2 = 36; //links
static const int USsensorFrontEcho2 = 38;
//left
static const int USsensorLeftTrigger1 = 44;
static const int USsensorLeftEcho1 = 42;
int leftDistance = 0;
int leftDistancearr = 0;
//right
static const int USsensorRightTrigger1 = 37;
static const int USsensorRightEcho1 = 35;
//back
static const int USsensorBackTrigger1 = 39;
static const int USsensorBackEcho1 = 41;
int backDistance = 0;
int backDistancearr = 0;

//servo2
#include <SoftwareSerial.h>
#include "MeOrion.h"
#include "MeSmartServo.h"
#include <AccelStepper.h>

MeSmartServo mysmartservo(PORT5);   //UART2 is on port 5

long loopTime = 0; 
bool dir = LOW; //stepper off

const int mtr_speed = 20;
const int dirPin = 43;
const int stepPin = 45;
const int mosfet = 47;

const int correctServ1 = 10;
const int correctServ2 = -45+10;
const int correctServ3 = 0;
//end servo

struct PositionStruct {
  double x_pos;
  double y_pos;
  float heading;
};

byte ledPin = 13;   // the onboard LED

int counter = 0;
int statement = 0;
int heading_statement = 0;
float heading_doel;


void setup() {
  Serial.begin(115200);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  /*
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  */
  
  pinMode(USsensorFrontTrigger1, OUTPUT);
  pinMode(USsensorFrontEcho1, INPUT);
  pinMode(USsensorFrontTrigger2, OUTPUT);
  pinMode(USsensorFrontEcho2, INPUT);
  pinMode(USsensorLeftTrigger1, OUTPUT);
  pinMode(USsensorLeftEcho1, INPUT);
  pinMode(USsensorRightTrigger1, OUTPUT);
  pinMode(USsensorRightEcho1, INPUT);
  pinMode(USsensorBackTrigger1, OUTPUT);
  pinMode(USsensorBackEcho1, INPUT);
  
  mysmartservo.begin(115200);
  delay(5);
  mysmartservo.assignDevIdRequest();
  delay(50);
  loopTime = millis();
  pinMode(mosfet,OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  for (int i = 0; i < 10; i++) {
    backDistancearr += measureDist(USsensorBackTrigger1, USsensorBackEcho1);  //x start
    //leftDistancearr += measureDist(USsensorLeftTrigger1, USsensorLeftEcho1);  //y start
    Serial.print(i);
    Serial.print(" , ");
    Serial.println(backDistancearr );
  }
  backDistance = backDistancearr / 10;
  //leftDistance = leftDistancearr / 20;
  Serial.print("back distance: ");
  Serial.println(backDistance);
  /*
  Serial.println("<left distance:>");
  Serial.print(leftDistance);
  */
  Serial.println("<Arduino is ready>");
}
//waardes van robot
float headingingraden= 0 * (3.14159265359 / 180);
//PositionStruct Position = {leftDistance, backDistance, headingingraden}; // x en y zijn y en x in speelveld, dus omdraaien hier
PositionStruct Position = {0, 0, 0}; // x en y zijn y en x in speelveld, dus omdraaien hier

int emptyserial, id_ard, x_ard, y_ard, heading_ard = 0;

//locaties van de samples
int x_sample[] = {40, 60, 0};
int y_sample[] = {30, 70, 0};
int samples_place[] = {3, 3};
int heading_sample[] = {0, 0};
int sample_counter = 0; // bepalen of samples palced >=3

//waardes voor station
double x_station = 5 ;
double y_stationRed = 33;
double y_stationBlue = 100;
double y_stationGreen = 166;

// verschillende states
int state_ExcavationSquare = 0; //0 = niet bij excavation square, 1 = bij excavation square, 2 = bij turnoversquare
int state_sample = 0; //0 = sample opgenomen, 1 = geen sample opgenomen
int state_station = 0; //0 = niet bij station, 1 = bij station
//state voor serial communication
int state_serial = 0; //0 = serial niet ontvangen, 1 = serial ontvangen

//waardes van mapp
double camera_y = 20;
float camera_heading = 60; // * (3.14159265359/180);

//Serial communication rpi - ard
String data = "";


void loop(){
  /*
  odometry(Position); //update positie
  
  if (Serial.available() > 0 && state_serial == 0) { //receive data from raspberry pi
    //recvWithStartEndMarkers();
    
    data = Serial.readStringUntil('\n');
    //sscanf(data.c_str(), "%d;%d;%d;%d;%d", &emptyserial, &id_ard, &x_ard, &y_ard, &heading_ard);    //add emptyserial because the first value doesn't get sent/ received
    //Serial.print(id_ard); Serial.print(","); Serial.print(x_ard); Serial.print(",");Serial.print(y_ard); Serial.print(","); Serial.println(heading_ard);
    //Serial.println(data);
    //if (id_ard != 0) { state_serial = 1;}
  }
  sscanf(data.c_str(), "%d;%d;%d;%d;%d", &emptyserial, &id_ard, &x_ard, &y_ard, &heading_ard);    //add emptyserial because the first value doesn't get sent/ received
  
  //Serial.print(id_ard); Serial.print(","); Serial.print(x_ard); Serial.print(",");Serial.print(y_ard); Serial.print(","); Serial.println(heading_ard);
  Serial.println(id_ard);
  if (id_ard > 0){ digitalWrite(led1, HIGH);} //6
  if (x_ard > 0){ digitalWrite(led2, HIGH);} //7
  if (y_ard > 0){ digitalWrite(led3, HIGH);} //8
  if (heading_ard > 0){ digitalWrite(led4, HIGH);} //9
  
  if (id_ard == -1) { //mogelijks nog een waarde erbij doen indien id_ard gereset wordt wanneer de camera de sample niet meer ziet
    state_ExcavationSquare = 2;
    state_sample = 2;
    state_station = 2;

    Serial.println("Time's up");
    if (heading_statement == 0) { //hoek bepalen
    heading_doel = heading_station(Position.x_pos,  Position.y_pos,  backDistance,  leftDistance); //berekenen van de hoek tussen huidige positie en station
    heading_statement = 1;
    }
    goTo(Position.x_pos, Position.y_pos, backDistance, leftDistance ,Position.heading, heading_doel);
  }  

  odometry(Position); //update positie

  int frontDistance = measureDist(USsensorFrontTrigger1, USsensorFrontEcho1);
  int frontDistance2 = measureDist(USsensorFrontTrigger2, USsensorFrontEcho2);
  if ((frontDistance <10 || frontDistance2< 10) && Position.x_pos > 20) { // niet botsen
    Serial.println("niet botsen");
    stilstand();
  }
  if (samples_place[0] < 1){ // go from excavation square to turnover square
    odometry(Position); //update positie
    Serial.println("All samples in excavation1 place stored. Searching in excavation2/turn square");
    sample_counter = 1;
  }
  // go to sample state_ExcavationSquare
  if (state_ExcavationSquare  == 0  && state_sample == 0 && state_station == 0) {
    //Serial.println("go to excavation square");
    Serial.println(data);
    odometry(Position); //update positie
    goTo(Position.x_pos,  Position.y_pos,  x_sample[sample_counter],  y_sample[sample_counter],  Position.heading, heading_sample[sample_counter]);
    if (Position.x_pos > x_sample[sample_counter] - 5 && Position.x_pos < x_sample[sample_counter] + 5 && Position.y_pos > y_sample[sample_counter] - 5 && Position.y_pos < y_sample[sample_counter] + 5){
      state_ExcavationSquare = 1;
    }
  }
  // stand still and check for sample
  if (state_ExcavationSquare  != 0  && state_sample == 0 && state_station == 0) {
    Serial.println(" stand still and check for sample");
    odometry(Position); //update positie
    stilstand();
    if (id_ard != 0) {
      //move to sample
      Serial.println("move to sample");
      if ( heading_statement == 0) { //hoek bepalen + afstand berekenen
        odometry(Position); //update positie
        heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_station,  y_stationBlue); //berekenen van de hoek tussen huidige positie en station
        heading_statement = 1;
        x_ard += Position.x_pos;
        y_ard += Position.y_pos;
      }
      odometry(Position); //update positie
      goTo(Position.x_pos, Position.y_pos, x_ard, y_ard ,Position.heading, heading_doel);
    }
    //grab sample
    if (Position.x_pos > x_ard - 5 && Position.x_pos < x_ard + 5 && Position.y_pos > y_ard - 5 && Position.y_pos < y_ard + 5){
      odometry(Position); //update positie
      Serial.println("grab sample");
      stilstand();
      if (id_ard == 17){  //misschien naar while veranderen maar dan gaat serial comm niet worden uitgevoerd. of aanpassen en in de loop expliciet communicateie oproepen
        odometry(Position); //update positie
        grabbingSample17();
        state_serial = 0; // check for new id
      }
      else{
        grabbingSample();
        odometry(Position); //update positie
      }
      odometry(Position); //update positie
      state_sample = 1;
      heading_statement = 0;
    }
  }


  //depending on ard_id, go to blue station
  if (id_ard == 13 && state_ExcavationSquare != 0  && state_sample == 1 && state_station == 0){ //blue
    Serial.println("to blue station");
    odometry(Position); //update positie
    if ( heading_statement == 0) { //hoek bepalen
      odometry(Position); //update positie
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_station,  y_stationBlue); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
    }
    goTo(Position.x_pos, Position.y_pos, x_station, y_stationBlue, Position.heading, heading_doel);
    if (Position.x_pos > x_station - 5 && Position.x_pos < x_station + 5 && Position.y_pos > y_stationBlue - 5 && Position.y_pos < y_stationBlue + 5){
      state_station = 1;
      odometry(Position); //update positie
    }
  }
  //depending on ard_id, go to red station
  if (id_ard == 47 && state_ExcavationSquare  != 0  && state_sample == 1 && state_station == 0){ //red
    Serial.println("to red station");
    if ( heading_statement == 0) { //hoek bepalen
      odometry(Position); //update positie
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_station,  y_stationRed); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
    }
    odometry(Position); //update positie
    goTo(Position.x_pos, Position.y_pos, x_station, y_stationRed, Position.heading, heading_doel);
    if (Position.x_pos > x_station - 5 && Position.x_pos < x_station + 5 && Position.y_pos > y_stationRed - 5 && Position.y_pos < y_stationRed + 5){
      odometry(Position); //update positie
      state_station = 1;
    }
  }
  //depending on ard_id, go to green station
  if (id_ard == 36 && state_ExcavationSquare  != 0 && state_sample == 1 && state_station == 0){ //green
    Serial.println("to green station");
    odometry(Position); //update positie
    if ( heading_statement == 0) { //hoek bepalen
      odometry(Position); //update positie
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_station,  y_stationGreen); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
    }
    goTo(Position.x_pos, Position.y_pos, x_station, y_stationGreen, Position.heading, heading_doel);
    if (Position.x_pos > x_station - 5 && Position.x_pos < x_station + 5 && Position.y_pos > y_stationGreen - 5 && Position.y_pos < y_stationGreen + 5){
      state_station = 1;
      odometry(Position); //update positie
    }
  }
  //drop sample
  if (state_ExcavationSquare  == 1 && state_sample == 1 && state_station == 1) {
    odometry(Position); //update positie
    Serial.println("drop sample");
    stilstand();
    if (sample_counter == 0){placeDownSample();}
    if (sample_counter == 1){placeDownSample180();}
    odometry(Position); //update positie
    state_sample = 0;
    state_station = 0;
    state_ExcavationSquare = 0;
    samples_place[sample_counter]++;
    heading_statement = 0;
    id_ard = 0;
  }
  */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//goTo( Position.x_pos,  Position.y_pos,  x_station,  y_stationGreen,  Position.heading, heading_doel ); //naar station rijden
//mapp( Position.x_pos,  Position.heading,  camera_y,  camera_heading); //naar sample rijden vanaf huidige positie
//mapp , mapping1 wordt (denk ik) nieet meer gebruikt

//watch out for grabbingsample17
