//########
//#Serial#
//#########

void SerialCommunication() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int str_len = data.length() + 1;
    char char_array[str_len];
    data.toCharArray(char_array, str_len);
    sscanf(data.c_str(), "%d;%d;%d;%d", &id_ard, &x_ard, &y_ard, &heading_ard);
    
    Serial.print("You sent me: ");
    Serial.print(data);
    Serial.print(" : ");
    Serial.print(id_ard);
    Serial.print(" & ");
    Serial.print(x_ard);
    Serial.print(Position.x_pos); //to test
    Serial.print(" & ");
    Serial.print(y_ard);
    Serial.print(" & ");
    Serial.println(heading_ard);
  }
}
