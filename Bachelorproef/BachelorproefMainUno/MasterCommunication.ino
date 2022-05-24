void standaardPosArm(){
   ArduinoSlave.print(String(90) + "&" + String(90) + "&" + String(0) + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "0");
}

void grabbingSample(double D, double E, double F){ // oproepen indien (x,y) coord rond (-3,-3) (marge van paar cm) liggen voor gewone samples
  ArduinoSlave.print(String(D) + "&" + String(E) + "&" + String(F) + "&" + String(D) + "&" + String(70) + "&" + String(F) + "&" + String("90") + "&" + String("90") + "&" + String(0) + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "1"); //volledige sequentie}
}
void grabbingSample17(double G, double H, double I){ // oproepen indien (x,y) coord rond (-3,-3) (marge van paar cm) liggen voor speciale sample
  ArduinoSlave.print(String(G) + "&" + String(H) + "&" + String(I) + "&" + String(G) + "&" + String(70) + "&" + String(I) + "&" + String(-45) + "&" + String(-35) + "&" + String(130) + "&" + String(90) + "&" + String(90) + "&" + String(0) + "&" + "1"); //servo(D,E,F); --> servo's laten bewegen op basis van (x,y)-coordinaten van camera indien de (x,y) coord rond (0,0) liggen;
}

void placeDownSample(){ // oproepen indien sample moet neergeplaatst worden (op dezelfde kant als deze is opgepakt)
  ArduinoSlave.print(String(-33) + "&" + String(-35) + "&" + String(-37) + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "0"); //servo(-33,-35,-37); --> standaard beweging om sample op grond te plaatsen
}

void placeDownSample180(){ // oproepen indien sample omgekeerd neergeplaatst moet worden
  ArduinoSlave.print(String(-33) + "&" + String(-45) + "&" + String(90) + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "#" + "&" + "0"); //servo(-33,-45,90); // standaard beweging om sample iets meer dan 90° te draaien zodat deze omgedraaid neervalt 
}

void readSlavePort(){
  while (ArduinoSlave.available()) {
    delay(10); 
    if (ArduinoSlave.available() >0) {
      char c = ArduinoSlave.read();  //gets one byte from serial buffer
      if (String(c) == "p"){ //because of 115200 baude, 0 turns into p
        msg += "0";
      }
      else if (String(c) == "&"){ //because of 115200 baude, x turns into ⸮
        msg += "x";
      }
      else {
        msg += c;
      }
    }
  }
  ArduinoSlave.flush();
}

int convertMsgToMultiCmd(){
  if (msg.length() >0) {
    data_count=0;
    do{
      sep = msg.indexOf('x');      
      String m1 = msg.substring(0, sep);
      msg = msg.substring(sep+1, msg.length());
      data[data_count]=m1;
      data_count++;
    } while(sep!=-1);
    if (data_count == 13){
      if (data[0] = "9999"){
        endOfServos = 1;
      }
    }
    msg="";
  }
  return endOfServos;
}
