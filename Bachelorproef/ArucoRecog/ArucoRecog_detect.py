### recognize tags and FOR NOW send the tag ID to promicro


# import the OpenCV library for computer vision
import cv2
import serial
import time
from threading import Thread

ser = None
listen_thread = None


def initSerial(handleMessage=None):     #serialcommunication from professor Jan Lemeire
    global ser, listen_thread   
    try:        
        ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    except:
        ser = None
        print('serial connection could not be started')
        return
    ser.flush()
    print('serial connection started')
    listen_thread = Thread(target = listen, args = [handleMessage])
    listen_thread.start()

continueListening = True
def listen(handleMessage=None):
    global ser, continueListening
    #prev_msg = ''
    while continueListening:
        line = str( ser.readline() ) #.decode('utf-8').rstrip()
        #msg = str(line) # 
        msg = line[2:len(line)-5]
        # prev_msg != msg:
        if msg is not None and len(msg) > 2:
            #print(msg)
        #prev_msg = msg
            if callable(handleMessage):
                handleMessage(msg)
        time.sleep(0.01)
    print('Finished listening. Closing serial.')
    ser.close()


""" test code
def insertcoord(x,y,z,servo, home = 0, handleMessage=None):
    global ser
    if ser is None:
        print('Initialization of serial connection.')
        initSerial(handleMessage)
        if ser is not None:
            time.sleep(25)

    try:
        ser.write(bytes(""+str(x)+";"+str(y)+";"+str(z)+";"+str(servo)+";"+str(home)+" \n", 'utf-8'))
        ser.flush()
    except:
        print('Sending command over serial failed...')
"""





def arucorec():
    """  Aruco code   """
    # Load the dictionary that was used to generate the markers.
    # There's different aruco marker dictionaries, this code uses 6x6
    dictionary = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)

    # Initialize the detector parameters using default values
    parameters = cv2.aruco.DetectorParameters_create()

    # initialize the webcam as "camera" object
    camera = cv2.VideoCapture(0)

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
            # for every tag in the array of detected tags...
            for i in range(len(markerIds)):

                print(markerIds[0])

                # get the center point of the tag
                center = markerCorners[i][0]
                M = cv2.moments(center)
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])
                # draws a red dot on the marker center
                cv2.circle(img, (cX, cY), 1, (0, 0, 255), 8)
                # writes the coordinates of the center of the tag
                cv2.putText(img, str(cX) + "," + str(cY), (cX + 40, cY - 40), cv2.FONT_HERSHEY_COMPLEX, 0.7,
                            (0, 255, 0), 2)
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

        """ Communication of aruco tag  """
        global ser
        if ser is None:
            print('Initialization of serial connection.')
            handleMessage=None
            initSerial(handleMessage)
            #initSerial()
            if ser is not None:
                time.sleep(25)
        try:
            ser.write(markerIds[0])
            ser.flush()
        except:
            print('Sending command over serial failed...')


        # Display the resulting frame
        cv2.imshow('frame', img)

        # handler to press the "q" key to exit the program
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


    # When everything done, release the capture
    camera.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    #ser = serial.Serial('/dev/ttyACM0', 1000000, timeout=1)
    #ser.flush()
    #time.sleep(12)
    arucorec()
    #insertcoord(22, 0, 0, 60, 0)
    #insertcoord(25, 1, 0, 60, 0)
    continueListening = False
    listen_thread.join()
