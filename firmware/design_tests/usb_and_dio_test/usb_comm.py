from __future__ import print_function
import sys
import serial
import time

class USBComm(serial.Serial):

    BaudRate = 115200 
    ResetDelay = 2.0 

    def __init__(self, port, timeout=120.0):
        super(USBComm,self).__init__(port, self.BaudRate,timeout=timeout)
        time.sleep(self.ResetDelay)

    def getData(self):
        self.write('d\n')
        rsp = self.readline()
        rsp = rsp.strip()
        rsp = rsp.split(' ')
        rsp = [int(x) for x in rsp]
        return rsp

    def getTime(self):
        self.write('t\n')
        rsp = self.readline()
        rsp = rsp.strip()
        rsp = int(rsp)
        return rsp

    def hello(self):
        print('hello')


# -----------------------------------------------------------------------------
if __name__ == '__main__':

    if len(sys.argv) > 1:
        port = sys.argv[1]
    else:
        port = '/dev/ttyACM0'


    dev = USBComm(port)

    for i in range(10):
        d = dev.getData()
        t = dev.getTime()
        print(i,d,t)
        time.sleep(0.1)


