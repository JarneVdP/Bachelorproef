# BachelorproefMainUno

Written by me:
- BachelorproefMainMega
- CommunicationRpi
- Odometer
- USSensor

Written by others:
- MasterCommunication
- Commandos
- Controls
- GoTo2
- GoTo3
- ToStation
  


To receive a message from the arduino at the raspberry pi, use *< ... >* in the arduino code when calling `Serial.print();`.

## Statemachine
---
1. Read ultrasonic sensors 10 times and take average for the sensor at the back and sensor at the left.

2. Go to excavation square at x: 70/50, y:130, 60 cm 


3. Stand still and ask the raspberry pi for the distance to the sample.

4. Camera responds with the distance to the sample.

5. Go to marker and take marker. If needed, flip the marker. Counter of markers left in the square-1

6. Drive with the marker to the correct station, depending on colour/Arucomarker. 

7. When the robot is at the station, drop the sample

8. Repeat






 