# Bachelorproef
Bachelorproef: Eurobot 2022

>What?

Code to control a robot which will take samples and store them by colour/ArucoId

>What does it do?

Go to a given location, take a sample which is recognised by a camera, go back to the start location, store sample and repeat.

>How will it work?

The movement and statemachine is on an arduino mega2560, the servos's vacuum and stepper motor are controlled by an arduino uno. The camera is connected to a raspberry pi. All of them are connected and can send messages to each other.


---
---
The github is subdivided in a couple of repositories. The first one is the master repository, BachelorproefMainMega. This is where the code is stored and executed by an arduino mega2560. The second one is the repository for the arduino uno, BachelorproefSlaveUno. The third one is the repository for the raspberry pi, ArucoRecog. The fourth one is the repository for the arduino mega2560. In Tests, different tests are combined. They can be used to test the camera, communicaton...

The code controls the robot ![autonomous robot](robot.jpeg)

Excuses for the dutch-english gibberish-esque comments  :smile:
