void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
        sscanf(receivedChars, "%d;%d;%d;%d;%d", &emptyserial, &id_ard, &x_ard, &y_ard, &heading_ard);    //add emptyserial because the first value doesn't get sent/ received
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void replyToPython() {
  if (newData == true) {
    Serial.print("<");
    Serial.print("turn");
    /*
    Serial.print(receivedChars);
    Serial.print("  , ");
    Serial.print(id_ard);
    Serial.print("  , x ");
    Serial.print(x_ard);
    Serial.print("  , y ");
    Serial.print(y_ard);
    Serial.print("  , pos heading ");
    Serial.print(Position.heading);
     Serial.print("  , heading doel ");
    Serial.print(heading_doel);
    */
    Serial.print(">");
    newData = false;
  }
}
