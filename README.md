# Bachelorproef
Bachelorproef: Eurobot 2022

Robot using arduino and raspberry pi
Raspberry pi for the aruco tag detection and localization
Arduino for motor, encoders/dead wheels for localization and other sensors(endstops, IR sensors...)

Aruco:
cam_cal: calibrate camera by taking pictures of a checkersboard
process: process distortion on the image/camera
camera_cal.npy: store the distortion

Aruco_detect_V2: distance to arucomarker from camera in real world: x, y , orientation/distance