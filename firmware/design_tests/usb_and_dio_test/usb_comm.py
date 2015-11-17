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


    def continuousReceive(self,num):

        # Flushout old data
        while (self.inWaiting() > 0):
            self.readline()
            
        cnt = 0
        self.ctlTimeList = []
        self.usbTimeList = []
        while cnt < num: 
            rsp = self.readline()
            self.write('t\n')
            rsp = rsp.strip()
            rsp = rsp.split(" ")
            haveData = False
            cnt += 1
            try:
                rsp = [int(x) for x in rsp]
                t0 = rsp[0]
                t1 = rsp[1]
                haveData = True 
            except:
                continue
            self.ctlTimeList.append(t0)
            self.usbTimeList.append(t1)
            print(cnt,t0,t1)
        return self.ctlTimeList, self.usbTimeList


# -----------------------------------------------------------------------------
if __name__ == '__main__':

    if len(sys.argv) > 1:
        port = sys.argv[1]
    else:
        port = '/dev/ttyACM0'


    dev = USBComm(port)

    for i in range(100):
        d = dev.getData()
        t = dev.getTime()
        print(i,d,t)
        time.sleep(0.1)


