//LED pin
int LED = 13;


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}



void loop() {
  // Check if there is anything in the Serial Buffer
  if (Serial.available())
  {
    // Read one value from the Serial buffer and store it in the variable com
    int com = Serial.read();
    // Act according to the value received
    if (com == 'x')
    {
      // stop the led
      digitalWrite(LED, LOW);
    }
    else if (com == 'a')
    {
      // Start the LED
      digitalWrite(LED, HIGH);
    }
  }
}
