#include <SoftwareSerial.h>
#include "MeOrion.h"
#include "MeSmartServo.h"

#define rxPin 8         //for RX: only [8,10,9,11]
#define txPin 6         //for TX: only [6,7,4,2]

SoftwareSerial ArduinoMaster(rxPin, txPin); //RX,TX
MeSmartServo mysmartservo(PORT5);   //UART.2 is on port 5

//alles voor softserial comm en servo
int intVal1=0,oldIntVal1=0;
int intVal2=0,oldIntVal2=0;
String msg;
int ledVal=0;
int sep;
String data[13];
unsigned int data_count=0;
int Ack_tx = 2; // Acknowledgement tx.
long loopTime = 0;

//pinnen voor zuigsysteem en stepper
bool dir = LOW;
const int mtr_speed = 20;
const int dirPin = 12;
const int stepPin = 13;
const int mosfet = A3;

//correcties voor servo's
const int correctServ1 = 10;
const int correctServ2 = -45+10;
const int correctServ3 = 0;

void setup(){
  ArduinoMaster.begin(115200);
  Serial.begin(115200);
  mysmartservo.begin(115200);
  delay(5);
  mysmartservo.assignDevIdRequest();
  delay(50);
  loopTime = millis();
  pinMode(mosfet,OUTPUT);
//  pinMode(stepPin, OUTPUT);
//  pinMode(dirPin, OUTPUT);
}

void loop(){
  readMasterPort(); // lees msg van softserial comm
  convertMsgToMultiCmd(); // convert msg en beweeg servo's
//  MotorGoUp(20);
//  digitalWrite(mosfet,HIGH);
}

void readMasterPort(){
  while (ArduinoMaster.available()) {
    delay(10); 
    if (ArduinoMaster.available() >0) {
      char c = ArduinoMaster.read();  //gets one byte from serial buffer
      if (String(c) == "p"){ //because of 115200 baude, 0 turns into p
        msg += "0";
      }
      else if (String(c) == "&"){ //because of 115200 baude, x turns into ⸮
        msg += "x";
      }
      else {
        msg += c; //makes the string readString
      }
    }
  }
  ArduinoMaster.flush();
}

void convertMsgToMultiCmd(){
  if (msg.length() >0) {
    data_count=0;
    do{
      sep = msg.indexOf('x');
      // expect a string like 0x0021 containing the two servo positions      
      String m1 = msg.substring(0, sep); //get servo id
      msg = msg.substring(sep+1, msg.length()); //get servo pos 
      data[data_count]=m1;
      data_count++;
    } while(sep!=-1);
    for (int i=0;i<(data_count+1);i++) Serial.println(data[i]);
    if (data_count == 13){
    for (int i=0;i<(data_count+1);i++){
      switch(i){
        case 0:
        if (data[0] == "#"){
        Serial.println("HEKJE");
        msg="";
        }
        else {
          Serial.println("B1");
          servo(stringToInt(data[0]),stringToInt(data[1]),stringToInt(data[2]));
        }
        break;
        case 1:
        if (data[12] == "0"){
          Serial.println("ZUIG UIT");
          digitalWrite(mosfet,LOW);
          delay(1000);
        }
        else if (data[12] == "1"){
          Serial.println("ZUIG AAN");
          digitalWrite(mosfet,HIGH);
          delay(1000);
        }
        break;
        case 2:
        if (data[3] == "#"){
        Serial.println("HEKJE");
        msg="";
        }
        else {
          Serial.println("B2");
          servo(stringToInt(data[3]),stringToInt(data[4]),stringToInt(data[5]));
        }
        break;
        case 3:
        if (data[6] == "#"){
        Serial.println("HEKJE");
        msg="";
        }
        else {
          Serial.println("B3");
          servo(stringToInt(data[6]),stringToInt(data[7]),stringToInt(data[8]));
        }
        break;
        case 4:
        if (data[9] == "#"){
        Serial.println("HEKJE");
        msg="";
        }
        else {
          Serial.println("B4");
          servo(stringToInt(data[9]),stringToInt(data[10]),stringToInt(data[11]));
        }
        break;
        case 5:
        Serial.println("End"),
        ArduinoMaster.print("9999");
        break;
      }
    }
  }
    msg="";
  }
}

int stringToInt(String s){
  char carray1[6]; //magic needed to convert string to a number
  s.toCharArray(carray1, sizeof(carray1));
  return atoi(carray1);
}

void servo(int A, int B ,int C){
  mysmartservo.moveTo(1,A-correctServ1,35);
  mysmartservo.moveTo(2,B-correctServ2,35);
  mysmartservo.moveTo(3,C-correctServ3,35);
  loopTime = millis();
  while(millis() - loopTime < 1500){}
}

void MotorGoUp(int steps)
{
    digitalWrite(dirPin, HIGH);
    for(int i = 0; i<steps; i++)
    {
      digitalWrite(stepPin, HIGH);
      delay(mtr_speed);
      digitalWrite(stepPin, LOW);
      delay(mtr_speed);
    } 
}

void MotorGoDown(int steps)
{
    digitalWrite(dirPin, LOW);
    for(int i = 0; i<steps; i++)
    {
      digitalWrite(stepPin, HIGH);
      delay(mtr_speed);
      digitalWrite(stepPin, LOW);
      delay(mtr_speed);
    } 
}