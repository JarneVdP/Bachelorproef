#!/usr/bin/env python3
import serial
import time

""" 

def sendserial(idTag, x, y, heading):
    #ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1) #ttyACM0 USB0 ls /dev/tty*
    #ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #ttyACM0 USB0 ls /dev/tty*
    #ser.reset_input_buffer()
    ser.write(("%d;%d;%d;%d;%d\n" % (emptyserial ,idTag,x, y, heading)).encode('utf-8'))
    #starttime = time.time()
    #endtime = time.time()
    #while endtime - starttime < 5:
        #print(endtime - starttime)
        #line = ser.readline().decode('utf-8', errors='replace').rstrip()
        #print(line)
        #time.sleep(1)
        #endtime = time.time()
    #return line  

def recvserial():
    #ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1) #ttyACM0 USB0 ls /dev/tty*
    #ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) #ttyACM0 USB0 ls /dev/tty*
    #ser.reset_input_buffer()
    #ser.write(("%d;%d;%d;%d;%d\n" % (emptyserial ,idTag,x, y, heading)).encode('utf-8'))
    starttime = time.time()
    endtime = time.time()
    while endtime - starttime < 5:
        print(endtime - starttime)
        line = ser.readline().decode('utf-8', errors='replace').rstrip()
        print(line)
        #time.sleep(1)
        endtime = time.time()
    
"""   

startMarker = '<'
endMarker = '>'
dataStarted = False
dataBuf = ""
messageComplete = False


def setupSerial(baudRate, serialPortName):
    
    global  serialPort
    
    serialPort = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

    waitForArduino()

#========================
emptyserial=99
def sendToArduino(idTag, x, y, heading):
    
        # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPort
    
    stringWithMarkers = (startMarker)
    #stringWithMarkers += str(emptyserial)
    #stringWithMarkers += ";"
    #stringWithMarkers += str(idTag)
    #stringWithMarkers += ";"
    #stringWithMarkers += str(x)
    #stringWithMarkers += ";"
    stringWithMarkers += str(y)
    stringWithMarkers += ";"
    #stringWithMarkers += str(heading)
    stringWithMarkers += (endMarker)
    serialPort.write(stringWithMarkers.encode('utf-8')) # encode needed for Python3


#==================

def recvLikeArduino():

    global startMarker, endMarker, serialPort, dataStarted, dataBuf, messageComplete

    if serialPort.inWaiting() > 0 and messageComplete == False:
        x = serialPort.read().decode("utf-8", errors='ignore') # decode needed for Python3
        
        if dataStarted == True:
            if x != endMarker:
                dataBuf = dataBuf + x
            else:
                dataStarted = False
                messageComplete = True
        elif x == startMarker:
            dataBuf = ''
            dataStarted = True
    
    if (messageComplete == True):
        messageComplete = False
        return dataBuf
    else:
        return "XXX" 

#==================

def waitForArduino():
 
    print("Waiting for Arduino to reset")
     
    msg = ""
    while msg.find("Arduino is ready") == -1:
        msg = recvLikeArduino()
        if not (msg == 'XXX'): 
            print(msg)


"""
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()
    starttime = time.time()
    endtime = time.time()
    while True:
        if endtime - starttime < 3:
            #print(endtime - starttime)
            ser.write(("%d;%d;%d;%d;%d\n" % (emptyserial ,idTag,x, y, heading)).encode('utf-8'))
            line = ser.readline().decode('utf-8', errors='ignore').rstrip()
            print(line)
            endtime = time.time()
        print("done")
"""

if __name__ == '__main__':
    setupSerial(9600, "/dev/ttyACM0")
    prevTime = time.time()
    while True:
                # check for a reply
        arduinoReply = recvLikeArduino()
        #print(arduinoReply)
        if not (arduinoReply == 'XXX'):
            print("not xxx")
            #print ("Time %s  Reply %s" %(time.time(), arduinoReply))
            
            # send a message at intervals
        if time.time() - prevTime > 1.0:
            sendToArduino(1, 2, 3, 4)
            prevTime = time.time()
            

