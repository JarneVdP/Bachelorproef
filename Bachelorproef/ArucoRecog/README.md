# ArucoRecog

This folder is used to store the code for the recognition for the marker detection and distance calculation.
It also contains the code for the communication with the arduino.
A pdf with the needed Arucomarkers is included as well as a pdf to calibrate the camera.

Let's start at cam_cal.py and process.py.
>What is the purpose of cam_cal.py and process.py?

A camera lens is convex. We need to straighten the output of the camera. Print the checkerboard, run cam_cal.py which will take pictures. Take pictures of the flat checkerboard. Make sure all the pictures are clear. Run process.py. \
Congrats, you found your camera matrix and distortion. It's saved as camera_cal.npy.

>What is the purpose of the Aruco_detect_V2.py program?

The Aruco_detect_V2.py program is used to detect the markers and to calculate the distance between the markers. When the markers are detected and the arduino asks for it, 'turn'is received, send the markerID, x distance, y distance (and heading altough it isn't used at this moment).

> How can I run it?

Save everything locally on the raspberry pi. Locate the files in the folder. Go to the folder using cd .../.../ArucoRecog. Run the program using chmod +x Aruco_detect_V2.py, press enter. \
type ./Aruco_detect_V2.py, enter to run the program.