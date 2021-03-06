#!/usr/bin/env python3

# import the OpenCV library for computer vision
import cv2
import serial
import time, sys, math
import numpy as np

camera_width = 640
camera_height = 480

def arucorec():
    """  Aruco code   """
    # Load the dictionary that was used to generate the markers.
    # There's different aruco marker dictionaries, this code uses 6x6
    dictionary = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)

    # Initialize the detector parameters using default values
    parameters = cv2.aruco.DetectorParameters_create()

    # initialize the webcam as "camera" object
    camera = cv2.VideoCapture(-1, cv2.CAP_V4L)

    # loop that runs the program forever
    # at least until the "q" key is pressed
    while True:

        # creates an "img" var that takes in a camera frame
        _, img = camera.read()
        
        img = cv2.rotate(img, cv2.ROTATE_180)

        # Convert to grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # detect aruco tags within the frame
        markerCorners, markerIds, rejectedCandidates = cv2.aruco.detectMarkers(gray, dictionary, parameters=parameters)

        # draw box around aruco marker within camera frame
        img = cv2.aruco.drawDetectedMarkers(img, markerCorners, markerIds)
        
        # if a tag is found...
        if markerIds is not None:
            """ Arduino communication send and receive(to see if what has been sent is correct). Probably chgange it to send the distances + markerId every (half) second"""
            #ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
            #ser.reset_input_buffer()
            #ser.write('{}'.format(markerIds).encode())
            #ser.flush()
            #line = ser.readline().decode('utf-8').rstrip()
            #print(line)

            #print(markerIds[0], markerIds)
            for i in range(len(markerIds)):
            # get the center point of the tag
                center = markerCorners[i][0]
                M = cv2.moments(center)
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])
                # writes the coordinates of the center of the tag
                cv2.putText(img,"x:"+ str(cX) + ", y:" + str(cY), (cX, cY), cv2.FONT_HERSHEY_TRIPLEX, 0.7,
                            (0, 255, 0), 2) #center cX + 40, cY - 40, we use 50 mm markers ,(cX-25, cY-25)
            
            """
            # draws a red dot on the marker center
            #cv2.circle(img, (cX, cY), 1, (0, 0, 255), 8)
            # for every tag in the array of detected tags...
            for i in range(len(markerIds)):

                (topLeft, topRight, bottomRight, bottomLeft) = markerCorners[i][0]
                # convert each of the (x, y)-coordinate pairs to integers
                topRight = (int(topRight[0]), int(topRight[1]))
                bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
                bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
                topLeft = (int(topLeft[0]), int(topLeft[1]))

                # draw the bounding box of the ArUCo detection
                cv2.line(img, topLeft, topRight, (0, 255, 0), 2)
                cv2.line(img, topRight, bottomRight, (0, 255, 0), 2)
                cv2.line(img, bottomRight, bottomLeft, (0, 255, 0), 2)
                cv2.line(img, bottomLeft, topLeft, (0, 255, 0), 2)
            """

        # Display the resulting frame
        cv2.imshow('frame', img)

        # handler to press the "q" key to exit the program
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        

    # When everything done, release the capture
    camera.release()
    cv2.destroyAllWindows()
    return markerIds[0]



if __name__ == '__main__':
    arucorec()
