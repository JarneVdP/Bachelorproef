#!/usr/bin/env python3
  
import cv2
# define a video capture object
#cap = "gst-launch-1.0 libcamerasrc ! video/x-raw, width=1280, height=720, framerate=30/1 ! videoconvert ! videoscale ! video/x-raw, width=640, height=360 ! autovideosink"
vid = cv2.VideoCapture(0)
  
while(True):
      
    # Capture the video frame
    # by frame
    ret, frame = vid.read()
    print(ret, frame)
    # Display the resulting frame
    cv2.imshow('frame', frame)
      
    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
  
# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()