#!/usr/bin/env python3
from matplotlib.pyplot import gray
import numpy as np
import cv2, math, time, serial
import datetime as dt

t = dt.datetime.now()

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

def sendserial(idTag, x, y, heading):
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()
    #while True:
    ser.write(str.encode("%d;%d;%d;%d\n" % (idTag[0][0],realworld_tvec[0], realworld_tvec[1], math.degrees(yaw))))
    #ser.write(str.encode("%s;%s;%s\n" % (idTag[0][0], realworld_tvec[1], math.degrees(yaw))))
    line = ser.readline().decode('utf-8').rstrip()
    print(line)
    time.sleep(1)

marker_size = 50        
with open('camera_cal.npy', 'rb') as f:
    camera_matrix = np.load(f)
    camera_distortion = np.load(f)

aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)

cap = cv2.VideoCapture(0)

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

    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    corners, ids, rejected = cv2.aruco.detectMarkers(gray_frame, aruco_dict, camera_matrix, camera_distortion)

    if ids is not None:
        cv2.aruco.drawDetectedMarkers(frame, corners)

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

        if math.degrees(yaw) > 0.0 and math.degrees(yaw) < 90.0:
            send_x = realworld_tvec[1]
            send_y = realworld_tvec[0]
            send_heading = math.degrees(yaw)
        if math.degrees(yaw) > 90.0 and math.degrees(yaw) < 180.0:
            send_x = realworld_tvec[0]
            send_y = realworld_tvec[1]
            send_heading = math.degrees(yaw) - 90
        if math.degrees(yaw) < 0.0 and math.degrees(yaw) < -90.0:
            send_x = -realworld_tvec[0]
            send_y = -realworld_tvec[1]
            send_heading = math.degrees(yaw)
        if math.degrees(yaw) < -90.0 and math.degrees(yaw) < -180.0:
            send_x = -realworld_tvec[1]
            send_y = -realworld_tvec[0]
            send_heading = math.degrees(yaw) + 90   
        
        for i in range(len(ids)):
            # get the center point of the tag
            center = corners[i][0]
            M = cv2.moments(center)
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])

        tvec_str = "id=%s x=%4.0f  y=%4.0f  dir=%4.0f"%(ids, send_x, send_y, send_heading)
        cv2.putText(frame, tvec_str, (20, 460), cv2.FONT_HERSHEY_TRIPLEX, 0.7, (0, 0, 255), 2)
        
        # If ten second passes, send coordinates
        delta = dt.datetime.now()-t
        if delta.seconds >= 2:
            sendserial(ids, realworld_tvec[0], realworld_tvec[1], math.degrees(yaw))
            t = dt.datetime.now()

    cv2.imshow('frame', frame)
    

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

cap.release()
cv2.destroyAllWindows

""" rijden totdat x = 0 <= gecenterd voor de sample"""
"""rijden totdat y = ..."""
""" op dit moment nog niet teveel naar de angle kijken van de camera, enkel naar de angle van de encoderes om zichzelf recht te zetten."""


""" get real world pose from aruco marker """ 
""" https://www.youtube.com/watch?v=cIVZRuVdv1o """