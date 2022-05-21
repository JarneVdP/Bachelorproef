#!/usr/bin/env python3
import serial
import time

emptyserial = -99
idTag =  1
x = 2
y = 3
heading = 4

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()
    starttime = time.time()
    endtime = time.time()
    while endtime - starttime < 3:
        ser.write(("%d;%d;%d;%d;%d\n" % (emptyserial ,idTag,x, y, heading)).encode('utf-8'))
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        time.sleep(1)
        endtime = time.time()
        #time.sleep(60.0 - ((time.time() - starttime) % 60.0))
    print("done")
    

