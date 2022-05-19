const byte numChars = 64;
char receivedChars[numChars];

boolean newData = false;

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
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    //sscanf(data.c_str(), "%d;%d;%d;%d;%d", &emptyserial, &id_ard, &x_ard, &y_ard, &heading_ard);    //add emptyserial because the first value doesn't get sent/ received
}

//===============

void replyToPython() {
    if (newData == true) {
        Serial.print("<This just in ... ");
        Serial.print(receivedChars);
        Serial.print('>');
        newData = false;
    }
}

//===============
