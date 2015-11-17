from __future__ import print_function
import sys
import usb_comm
import scipy
import matplotlib.pyplot as plt
plt.ion()
from PyQt4 import QtCore
from PyQt4 import QtGui
from usb_timer_test_ui import Ui_MainWindow


class USBTimerTest(QtGui.QMainWindow, Ui_MainWindow):

    def __init__(self,port, parent=None):
        super(USBTimerTest,self).__init__(parent)
        self.setupUi(self)
        self.freq = 100.0
        self.running = False

        self.startStopPushButton.clicked.connect(self.startStopClicked_Callback)
        self.updateStartStopText()

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.onTimer)

        self.dev = usb_comm.USBComm(port)
        self.outfile = None
        self.outfileName = 'data.txt'
        self.diffTimeList = []
        self.timeList = []
        self.count = 0

    def main(self):
        self.show()
        self.raise_()

    def startStopClicked_Callback(self):
        print('start/stop clicked')
        if self.running:
            self.timer.stop()
            self.running = False
            self.outfile.close()

            t = scipy.array(self.timeList)
            dt = (t[1:] - t[:-1])*1e-6
            dt = dt[100:]
            dtMean = dt.mean()
            dtMedian = scipy.median(dt)
            dtStdDev = dt.std()
            dtMedianAbsDev = scipy.median(scipy.absolute(dt - dtMedian))
            dtMaxAbsDev = scipy.absolute(dt - dtMean).max()
            dtMinAbsDev = scipy.absolute(dt - dtMean).min()
            numBin = int(0.01*dt.shape[0])
            numBin = max([numBin,20]) 

            diff = 1e-6*scipy.array(self.diffTimeList)

            print()
            print('dt mean:           ', dtMean)
            print('dt median:         ', dtMedian)
            print('dt standard dev:   ', dtStdDev)
            print('dt medain abs dev: ', dtMedianAbsDev)
            print('dt max abs dev:    ', dtMaxAbsDev)
            print()

            fig = plt.figure(1)
            plt.clf()
            plt.hist(dt,numBin)
            plt.xlabel('Period dt (sec)')
            plt.ylabel('Count')
            plt.grid('on')
            plt.draw()

            #fig = plt.figure(2)
            #plt.clf()
            #plt.hist(diff,numBin)
            #plt.xlabel('Get-Set dt (sec)')
            #plt.ylabel('Count')
            #plt.grid('on')
            #plt.draw()

        else:
            self.running = True 
            self.count = 0
            self.timeList = []
            self.outfile = open(self.outfileName,'w')
            self.timer.start(1.0e3/self.freq)
        self.updateStartStopText()

    def updateStartStopText(self):
        if self.running:
            self.startStopPushButton.setText('Stop');
        else:
            self.startStopPushButton.setText('Start');

    def onTimer(self):
        t0 = self.dev.getTime()
        #t1 = self.dev.getTime()
        t1 = t0
        self.timeList.append(t0)
        self.diffTimeList.append(t1 - t0)
        self.count += 1
        self.outfile.write('{0} {1} {2}\n'.format(self.count,t0,t1))
        print(self.count, t1, t1-t0)


def runApp(port):
    app = QtGui.QApplication(sys.argv)
    mainWindow = USBTimerTest(port)
    mainWindow.main()
    app.exec_()

# -----------------------------------------------------------------------------
if __name__ == '__main__':

    if len(sys.argv) > 1:
        port = sys.argv[1]
    else:
        port = '/dev/ttyACM0'

    runApp(port)
