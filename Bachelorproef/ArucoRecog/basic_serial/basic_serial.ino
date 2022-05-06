void setup() {
  Serial.begin(9600);
}

int x_ard, y_ard, heading_ard, id_ard;

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    sscanf(data.c_str(), "%d;%d;%d;%d", &id_ard, &x_ard, &y_ard, &heading_ard);
    
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
