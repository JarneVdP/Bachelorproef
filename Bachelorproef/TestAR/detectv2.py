#!/usr/bin/env python3
from matplotlib.pyplot import gray
import numpy as np
import cv2, math, time, serial
import datetime as dt
import cv2.aruco as aruco

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
    ser.write(str.encode("%d;%d;%d;%d\n" % (idTag[0][0],x, y, heading)))
    line = ser.readline().decode('utf-8').rstrip()
    print(line)
    time.sleep(1)

def convertXY(yaw, realworld_tvec):
    if math.degrees(yaw) > 0.0 and math.degrees(yaw) < 90.0:
        send_x = realworld_tvec[1]
        send_y = realworld_tvec[0]
        send_heading = math.degrees(yaw)
    if math.degrees(yaw) > 90.0 and math.degrees(yaw) < 180.0:
        send_x = realworld_tvec[0]
        send_y = realworld_tvec[1]
        send_heading = math.degrees(yaw) 
    if math.degrees(yaw) < 0.0 and math.degrees(yaw) > -90.0:
        send_x = -realworld_tvec[0]
        send_y = -realworld_tvec[1]
        send_heading = math.degrees(yaw)
    if math.degrees(yaw) < -90.0 and math.degrees(yaw) > -180.0:
        send_x = realworld_tvec[1]
        send_y = -realworld_tvec[0]
        send_heading = math.degrees(yaw) 
    return send_x, send_y, send_heading


def findArucoMarkers(img, markerSize = 4, totalMarkers=250, draw=True):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    key = getattr(aruco, f'DICT_{markerSize}X{markerSize}_{totalMarkers}')
    arucoDict = aruco.Dictionary_get(key)
    arucoParam = aruco.DetectorParameters_create()
    bboxs, ids, rejected = aruco.detectMarkers(gray, arucoDict, parameters = arucoParam)
    #print(ids)
    if ids is not None:
        rvec_list_all, tvec_list_all, _objPoints = cv2.aruco.estimatePoseSingleMarkers(bboxs, marker_size, camera_matrix, camera_distortion)
        
        rvec = rvec_list_all[0][0]
        tvec = tvec_list_all[0][0]

        rvec_flipped = rvec*-1
        tvec_flipped = tvec*-1
        rotation_matrix, jacobian = cv2.Rodrigues(rvec_flipped)
        realworld_tvec = np.dot(rotation_matrix, tvec_flipped)

        pitch, roll, yaw = rotationMatrixToEulerAngles(rotation_matrix)
        send_x, send_y, send_heading = convertXY(yaw, realworld_tvec)
        tvec_str = "id=%s x=%4.0f  y=%4.0f  dir=%4.0f "%(ids, send_x, send_y, send_heading)
        cv2.putText(img, tvec_str, (20, 460), cv2.FONT_HERSHEY_TRIPLEX, 0.7, (0, 0, 255), 2)
        print(tvec_str)
        #print(ids, int(realworld_tvec[0]), int(realworld_tvec[1]), round(math.degrees(yaw)))
        print(rvec_list_all[0][0], tvec_list_all[0][0])
        aruco.drawDetectedMarkers(img, bboxs) 

marker_size = 50        
with open('camera_cal.npy', 'rb') as f:
    camera_matrix = np.load(f)
    camera_distortion = np.load(f)

cap = cv2.VideoCapture(0)
while True:
    success, img = cap.read()
    img = cv2.rotate(img, cv2.ROTATE_180)
    findArucoMarkers(img)
    cv2.imshow('img',img)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break
cap.release()
cv2.destroyAllWindows()