void setup() {
  Serial.begin(115200);
}

int emptyserial, x_ard, y_ard, heading_ard, id_ard = 0;

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    sscanf(data.c_str(), "%d;%d;%d;%d;%d", &emptyserial, &id_ard, &x_ard, &y_ard, &heading_ard);    //add emptyserial because the first value doesn't get sent/ received
    
    Serial.print("You sent me: ");
    Serial.print(data);
    Serial.print(" : ");
    Serial.print(id_ard);
    Serial.print(" & ");
    Serial.print(x_ard);
    Serial.print(" & ");
    Serial.print(y_ard);
    Serial.print(" & ");
    Serial.println(heading_ard);
  }
}
