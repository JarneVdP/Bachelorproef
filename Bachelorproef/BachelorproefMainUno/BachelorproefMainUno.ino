#include <SoftwareSerial.h>
#include <AccelStepper.h>

//Motor A
int enA = 8; //10
int in1 = 9; //9
int in2 = 7; //7
//Motor B
int enB = 6; //8
int in3 = 5; //5
int in4 = 4; //4
/* debugger leds*/
/*
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
int leftDistance;
int leftDistancearr = 0;
//right
static const int USsensorRightTrigger1 = 37;
static const int USsensorRightEcho1 = 35;
//back
static const int USsensorBackTrigger1 = 39;
static const int USsensorBackEcho1 = 41;
int backDistance;
int backDistancearr = 0;

//servo2 On arduino uno as slave
/*
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
  const int correctServ2 = -45 + 10;
  const int correctServ3 = 0;
*///end servo

struct PositionStruct {
  double x_pos;
  double y_pos;
  float heading;
};
PositionStruct Position = {0, 0, 0};

//Communication
const byte numChars = 64;
char receivedChars[numChars];

boolean newData = false;

int loopCounter = 0;
int statement = 0;
int heading_statement = 0;
float heading_doel;

//locaties van de samples
int x_sample[] = {80, 60, 0};
int y_sample[] = {125, 70, 0};
int samples_place[] = {3, 3};
int heading_sample[] = {0, 0};
int sample_counter = 0; // bepalen of samples palced >=3

//waardes voor station
double x_station = 40 ;
double y_stationRed = 50;
double y_stationBlue = 166;
double y_stationGreen = 100;

//Master communication and servo
const int rxPin = 50;
const int txPin = 49;
SoftwareSerial ArduinoSlave(rxPin, txPin); //RX,TX

String msg;
int sep;
String data[13];
unsigned int data_count = 0;
int endOfServos;

//station statements
int direction_bot = 0;
int position_bot = 0;
int direction_bott = 0;
int draai_bot = 0;
int stop_statement = 0;
int hoek_state = 0;


void setup() {
  Serial.begin(115200);
  ArduinoSlave.begin(115200);

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
  //Moet niet gebeuren op de mega
  /*
    mysmartservo.begin(115200);
    delay(5);
    mysmartservo.assignDevIdRequest();
    delay(50);
    loopTime = millis();
    pinMode(mosfet,OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
  */
  Serial.println("<Arduino is ready>");

  measureStartPos();
}

int emptyserial, id_ard, x_ard, y_ard, heading_ard;

// verschillende states
int state_ExcavationSquare = 0; //0 = niet bij excavation square, 1 = bij excavation square, 2 = bij turnoversquare
int state_sample = 0; //0 = sample opgenomen, 1 = geen sample opgenomen
int state_station = 0; //0 = niet bij station, 1 = bij station
//state voor serial communication
int state_serial = 2; //0 = serial niet ontvangen, 1 = serial ontvangen


void loop() {
  odometry(Position); //update positie
  /*
  if (loopCounter == 0) {
    measureStartPos();
    Serial.print("<x distance: ");
    Serial.println(Position.x_pos);
    Serial.print("y distance: ");
    Serial.println(Position.y_pos);
    Serial.println('>');
  }
  loopCounter = loopCounter + 1;
  */
  if (state_serial == 0) { //receive data from raspberry pi
    recvWithStartEndMarkers();
    if (id_ard != 0) {
      state_serial = 1;
    }
  }
  //replyToPython();
  delay(50);

  if (id_ard == -1) { //mogelijks nog een waarde erbij doen indien id_ard gereset wordt wanneer de camera de sample niet meer ziet
    state_ExcavationSquare = 2;
    state_sample = 2;
    state_station = 2;

    Serial.println("<Time's up>");
    if (heading_statement == 0) { //hoek bepalen
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,  backDistance,  leftDistance); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
    }
    goStation(Position.x_pos, Position.y_pos, backDistance, leftDistance , Position.heading, heading_doel);
  }

  odometry(Position); //update positie

  int frontDistance = measureDist(USsensorFrontTrigger1, USsensorFrontEcho1);
  int frontDistance2 = measureDist(USsensorFrontTrigger2, USsensorFrontEcho2);
  if ((frontDistance < 10 || frontDistance2 < 10) && Position.x_pos > 20) { // niet botsen
    Serial.println("<niet botsen>");
    stilstand();
  }
  if (samples_place[0] < 1) { // go from excavation square to turnover square
    odometry(Position); //update positie
    Serial.println("<All samples in excavation1 place stored. Searching in excavation2/turn square>");
    sample_counter = 1;
  }
  /*
  Serial.print("<state_ExcavationSquare ");
  Serial.println(state_ExcavationSquare);
  Serial.print("state_sample ");
  Serial.println(state_sample);
  Serial.print("state_station ");
  Serial.print(state_station);
  Serial.print(">");
  */
  // go to sample state_ExcavationSquare
  if (state_ExcavationSquare  == 0  && state_sample == 0 && state_station == 0) {
    //Serial.println("<go to excavation square>");
    
    odometry(Position); //update positie
    if ( heading_statement == 0) { //hoek bepalen + afstand berekenen
      odometry(Position); //update positie
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,   x_sample[sample_counter],  y_sample[sample_counter]); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
      delay(1000); standaardPosArm(); delay(1000);
      /*
      Serial.print("<heading_doel");
      Serial.println(heading_doel);
      Serial.print(">");
      */
    }
    goStation(Position.x_pos,  Position.y_pos,  x_sample[sample_counter],  y_sample[sample_counter],  Position.heading, heading_doel);
    if (Position.x_pos > x_sample[sample_counter] - 1 && Position.x_pos < x_sample[sample_counter] + 2) {
      state_ExcavationSquare = 1;
      heading_statement = 0;
      
      statement = 0;
      direction_bot = 0;
      position_bot = 0;
      direction_bott = 0;
      draai_bot = 0;
      stop_statement = 0;
    }
  }
  //replyToPython();
  delay(50);
  // stand still and check for sample
  if (state_ExcavationSquare  != 0  && state_sample == 0 && state_station == 0) {
    //Serial.println("<stand still and check for sample>");
    odometry(Position); //update positie
    
    if (hoek_state == 0){
      draai2(0, Position.heading, 0.01 , Position.x_pos, Position.x_pos );
      if (Position.heading > -(5 * 3.1415/180) && Position.heading < 5 * 3.1415 / 180){
        hoek_state = 1;
      }
    }
    if (hoek_state == 1){
      stilstand();
      state_serial = 0;
      hoek_state = 2;
      stop_statement = 0;
      statement = 0;
    }
    
    //replyToPython();
    delay(50);
    if (id_ard != 0 && hoek_state == 2) {
      //move to sample
      Serial.println("<move to sample>");
      if ( heading_statement == 0) { //hoek bepalen + afstand berekenen
        stilstand();
        odometry(Position); //update positie
        x_ard += Position.x_pos;
        y_ard += Position.y_pos;
        heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_ard,  y_ard); //berekenen van de hoek tussen huidige positie en station
        heading_statement = 1;
      }
      odometry(Position); //update positie
      goStation(Position.x_pos, Position.y_pos, x_ard - 7, y_ard , Position.heading, heading_doel);
    }

    //grab sample
    if (Position.x_pos > x_ard - 2 && Position.x_pos < x_ard + 2){  // && Position.y_pos > y_ard - 5 && Position.y_pos < y_ard + 5){
      odometry(Position); //update positie
      Serial.println("<grab sample>");
      stilstand();
      if (id_ard == 17) { //misschien naar while veranderen maar dan gaat serial comm niet worden uitgevoerd. of aanpassen en in de loop expliciet communicateie oproepen
        odometry(Position); //update positie
        delay(1000); grabbingSample17(-26.565, -38.612, -54.754); delay(1000);
        state_serial = 0; // check for new id
      }
      else {
        delay(2000); grabbingSample(-26.565, -38.612, -54.754); delay(2000);
        odometry(Position); //update positie
      }
      odometry(Position); //update positie
      state_sample = 1;
      heading_statement = 0;
      
      statement = 0;
      direction_bot = 0;
      position_bot = 0;
      direction_bott = 0;
      draai_bot = 0;
      stop_statement = 0;
    }
  }

  //depending on ard_id, go to blue station
  if (id_ard == 13 && state_ExcavationSquare != 0  && state_sample == 1 && state_station == 0) { //blue
    Serial.println("<to blue station>");
    odometry(Position); //update positie
    if ( heading_statement == 0) { //hoek bepalen
      odometry(Position); //update positie
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_station,  y_stationBlue); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
    }
    goStation(Position.x_pos, Position.y_pos, x_station, y_stationBlue, Position.heading, heading_doel);
    if (Position.x_pos < x_station - 2 && Position.x_pos < x_station + 2) {
      state_station = 1;
      odometry(Position); //update positie
    }
  }
  //depending on ard_id, go to red station
  if (id_ard == 47 && state_ExcavationSquare  != 0  && state_sample == 1 && state_station == 0) { //red
    Serial.println("<to red station>");
    if ( heading_statement == 0) { //hoek bepalen
      odometry(Position); //update positie
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_station,  y_stationRed); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
    }
    odometry(Position); //update positie
    goStation(Position.x_pos, Position.y_pos, x_station, y_stationRed, Position.heading, heading_doel);
    if (Position.x_pos > x_station - 5 && Position.x_pos < x_station + 2){      // && Position.y_pos > y_stationRed - 5 && Position.y_pos < y_stationRed + 5) {
      odometry(Position); //update positie
      state_station = 1;
    }
  }
  //depending on ard_id, go to green station
  if (id_ard == 36 && state_ExcavationSquare  != 0 && state_sample == 1 && state_station == 0) { //green
    Serial.println("<to green station>");
    odometry(Position); //update positie
    if ( heading_statement == 0) { //hoek bepalen
      odometry(Position); //update positie
      heading_doel = heading_station(Position.x_pos,  Position.y_pos,  x_station,  y_stationGreen); //berekenen van de hoek tussen huidige positie en station
      heading_statement = 1;
    }
    goStation(Position.x_pos, Position.y_pos, x_station, y_stationGreen, Position.heading, heading_doel);
    if (Position.x_pos > x_station - 2 && Position.x_pos < x_station + 2 ){ // && Position.y_pos > y_stationGreen - 5 && Position.y_pos < y_stationGreen + 5) {
      state_station = 1;
      odometry(Position); //update positie
    }
  }
  //drop sample
  if (state_ExcavationSquare  == 1 && state_sample == 1 && state_station == 1) {
    odometry(Position); //update positie
    Serial.println("<drop sample>");
    stilstand();
    delay(1000);
    placeDownSample();
    delay(1000);
    /*
    if (sample_counter == 0) {
      delay(1000);
      placeDownSample();
      delay(1000);
    }
    if (sample_counter == 1) {
      delay(1000);
      placeDownSample180();
      delay(1000);
    }
    */
    odometry(Position); //update positie
    state_sample = 0;
    state_station = 0;
    state_serial = 2;
    state_ExcavationSquare = 0;
    samples_place[sample_counter]++;
    heading_statement = 0;
    id_ard = 0;
    hoek_state = 0;

    statement = 0;
    direction_bot = 0;
    position_bot = 0;
    direction_bott = 0;
    draai_bot = 0;
    stop_statement = 0;
    x_ard = 0;
    y_ard = 0;
  }
}
