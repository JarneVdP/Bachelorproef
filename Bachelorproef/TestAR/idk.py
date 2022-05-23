#!/usr/bin/env python3
import serial
import time




def setupSerial(baudRate, serialPortName):
    
    global  serialPort
    
    serialPort = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

    waitForArduino()


emptyserial = 99
def sendToArduino(idTag, x, y, heading):
    
        # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPort
    
    stringWithMarkers = (startMarker)
    stringWithMarkers += str(emptyserial)
    stringWithMarkers += ";"
    stringWithMarkers += str(idTag)
    stringWithMarkers += ";"
    stringWithMarkers += str(x)
    stringWithMarkers += ";"
    stringWithMarkers += str(y)
    stringWithMarkers += ";"
    stringWithMarkers += str(heading)    
    stringWithMarkers += (endMarker)

    serialPort.write(stringWithMarkers.encode('utf-8')) # encode needed for Python3



def recvLikeArduino():

    global startMarker, endMarker, serialPort, dataStarted, dataBuf, messageComplete

    if serialPort.inWaiting() > 0 and messageComplete == False:
        x = serialPort.read().decode("utf-8") # decode needed for Python3
        
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



def waitForArduino():

    # wait until the Arduino sends 'Arduino is ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded
    
    print("Waiting for Arduino to reset")
     
    msg = ""
    while msg.find("Arduino is ready") == -1:
        msg = recvLikeArduino()
        if not (msg == 'XXX'): 
            print(msg)





setupSerial(9600, "/dev/ttyACM0")
prevTime = time.time()
while True:
            # check for a reply
    arduinoReply = recvLikeArduino()
    if not (arduinoReply == 'XXX'):
        print (" %s" %(arduinoReply))
        
        # send a message at intervals
        #sendToArduino("this is a test " + str(count))
        sendToArduino(1, 2, 3, 4)
        prevTime = time.time()
        