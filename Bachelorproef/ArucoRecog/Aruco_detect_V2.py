#!/usr/bin/env python3
from matplotlib.pyplot import gray
import numpy as np
import cv2, math, time, serial, sched
import datetime as dt


t = dt.datetime.now()
t_totaltime = dt.datetime.now()


# Checks if a matrix is a valid rotation matrix.    https://learnopencv.com/rotation-matrix-to-euler-angles/
def isRotationMatrix(R) :
    Rt = np.transpose(R)
    shouldBeIdentity = np.dot(Rt, R)
    I = np.identity(3, dtype = R.dtype)
    n = np.linalg.norm(I - shouldBeIdentity)
    return n < 1e-6

# Calculates rotation matrix to euler angles
# of the euler angles ( x and z are swapped ).
def rotationMatrixToEulerAngles(R) :

    assert(isRotationMatrix(R))

    sy = math.sqrt(R[0,0] * R[0,0] +  R[1,0] * R[1,0])

    singular = sy < 1e-6

    if  not singular :
        x = math.atan2(R[2,1] , R[2,2])
        y = math.atan2(-R[2,0], sy)
        z = math.atan2(R[1,0], R[0,0])
    else :
        x = math.atan2(-R[1,2], R[1,1])
        y = math.atan2(-R[2,0], sy)
        z = 0

    return np.array([x, y, z])

emptyserial = -99   #add a burner variable because the arduino doesn't read the first values
s = sched.scheduler(time.time, time.sleep)
t_ser_end = dt.datetime.now()
def sendserial(idTag, x, y, heading):
    #ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1) #ttyACM0 USB0 ls /dev/tty*
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #ttyACM0 USB0 ls /dev/tty*
    ser.reset_input_buffer()
    starttime = time.time()
    endtime = time.time()
    while endtime - starttime < 3:
        #idTag = 0
        ser.write(("%d;%d;%d;%d;%d\n" % (emptyserial ,idTag,x, y, heading)).encode('utf-8'))
        #ser.write("Hello from Raspberry Pi!\n".encode('utf-8'))
        line = ser.readline().decode('utf-8', errors='ignore').rstrip()
        #str = unicode(str, errors='replace')
        print(line)
        time.sleep(1)
        endtime = time.time()

    


with open('camera_cal.npy', 'rb') as f:
    camera_matrix = np.load(f)
    camera_distortion = np.load(f)

aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)

#change threshold to adjust the detection
parameters =  cv2.aruco.DetectorParameters_create()
parameters.adaptiveThreshConstant = 10
parameters.adaptiveThreshWinSizeMax = 15
parameters.adaptiveThreshWinSizeMin = 5
parameters.adaptiveThreshWinSizeStep = 2
parameters.cornerRefinementMethod = cv2.aruco.CORNER_REFINE_CONTOUR
parameters.cornerRefinementWinSize = 5
parameters.cornerRefinementMaxIterations = 30
parameters.errorCorrectionRate = 0.6


cap = cv2.VideoCapture(0)

marker_size = 5        
camera_width = 640
camera_height = 480
camera_frame_rate = 40


cap.set(2, camera_width)
cap.set(4, camera_height)
cap.set(5, camera_frame_rate)

send_x = 0
send_y = 0
send_heading = 0

while True:

    ret, frame = cap.read()
    frame = cv2.rotate(frame, cv2.ROTATE_180)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    #apply parameters on aruco detectMarkers
    corners, ids, rejected = cv2.aruco.detectMarkers(gray, aruco_dict, camera_matrix, camera_distortion)#, parameters=parameters)
    
    # if total time delta is bigger than 84, send data to arduino
    delta_totaltime = dt.datetime.now()-t_totaltime
    #if delta_totaltime.seconds >= 84:
        #sendserial(-1, 5, 5, math.degrees(0))
        #ids = None
        #print(tvec_str)

    if ids is not None:
        cv2.aruco.drawDetectedMarkers(frame, corners)
        """
        #V2:
        rvec_list_all, tvec_list_all, _objPoints = cv2.aruco.estimatePoseSingleMarkers(corners, marker_size, camera_matrix, camera_distortion)

        rvec = rvec_list_all[0][0]
        tvec = tvec_list_all[0][0]
        
        cv2.aruco.drawAxis(frame, camera_matrix, camera_distortion, rvec, tvec, 100)

        rvec_flipped = rvec*-1
        tvec_flipped = tvec*-1
        rotation_matrix, jacobian = cv2.Rodrigues(rvec_flipped)
        realworld_tvec = np.dot(rotation_matrix, tvec_flipped)

        pitch, roll, yaw = rotationMatrixToEulerAngles(rotation_matrix)

        #realworld_tvec[0] : x coördinaat
        #realworld_tvec[1] : y coördinaat


        distance = math.sqrt(realworld_tvec[0]**2 + realworld_tvec[1]**2) #te testen
        angle = math.degrees(math.atan2(realworld_tvec[0], realworld_tvec[1]))    #te testen

        #sample is to the right of the robot -> y is positive
        #sample is to the left of the robot -> y is negative
        
        if math.degrees(yaw) > 0.0 and math.degrees(yaw) < 90.0:
            send_x = realworld_tvec[1]
            send_y = -realworld_tvec[0] 
            send_heading = math.degrees(yaw)
        if math.degrees(yaw) > 90.0 and math.degrees(yaw) < 180.0:
            send_x = -realworld_tvec[1]
            send_y = realworld_tvec[0] 
            send_heading = math.degrees(yaw) 
        if math.degrees(yaw) < 0.0 and math.degrees(yaw) > -90.0:
            send_x = realworld_tvec[0]
            send_y = realworld_tvec[1] 
            send_heading = math.degrees(yaw)
        if math.degrees(yaw) < -90.0 and math.degrees(yaw) > -180.0:
            send_x = -realworld_tvec[1]
            send_y = realworld_tvec[0] 
            send_heading = math.degrees(yaw) 
    
        print(send_x, send_y)
        tvec_str = "id=%s x=%4.0f  y=%4.0f  dir=%4.0f"%(ids, rvec[0], tvec[0], delta_totaltime.seconds)
        cv2.putText(frame, tvec_str, (20, 460), cv2.FONT_HERSHEY_TRIPLEX, 0.7, (0, 0, 255), 2)
        """
        cameravisionX = 29
        cameravisionY = 38
        for i in range(len(ids)):
            # get the center point of the tag
                center = corners[i][0]
                M = cv2.moments(center)
                cY = int((M["m10"] / M["m00"]) / 10)
                cX = int((M["m01"] / M["m00"]) / 10)
                cY = cY * cameravisionY/60
                cY = int(cY) - int(cameravisionY/2)
                cX = cX * cameravisionX/42 
                cX = int(cX) - 10
                # writes the coordinates of the center of the tag
                cv2.putText(frame,"x:"+ str(cX) + ", y:" + str(cY), (20, 460), cv2.FONT_HERSHEY_TRIPLEX, 0.7,
                            (0, 255, 0), 2)


        # If two seconds pass, send coordinates
        delta = dt.datetime.now()-t
        if delta.seconds >= 1.5:
            #sendserial(ids, send_x, send_y, send_heading)
            #print(tvec_str)
            t = dt.datetime.now()

    cv2.imshow('frame', frame)
    

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows


""" get real world pose from aruco marker """ 
""" https://www.youtube.com/watch?v=cIVZRuVdv1o """
