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
        self.freq = 200.0
        self.running = False

        self.startStopPushButton.clicked.connect(self.startStopClicked_Callback)
        self.updateStartStopText()

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.onTimer)

        self.dev = usb_comm.USBComm(port)
        self.outfile = None
        self.outfileName = 'data.txt'
        self.timeLIst = []
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
            dt = (t[1:] - t[:-1])*1.e-6
            dtMean = dt.mean()
            dtMedian = scipy.median(dt)
            dtStdDev = dt.std()
            dtMedianAbsDev = scipy.median(scipy.absolute(dt - dtMedian))
            dtMaxAbsDev = scipy.absolute(dt - dtMean).max()
            dtMinAbsDev = scipy.absolute(dt - dtMean).min()

            print()
            print('dt mean:           ', dtMean)
            print('dt median:         ', dtMedian)
            print('dt standard dev:   ', dtStdDev)
            print('dt medain abs dev: ', dtMedianAbsDev)
            print('dt max abs dev:    ', dtMaxAbsDev)
            print()

            plt.clf()
            fig = plt.figure(1)
            numBin = int(max([0.01*dt.shape[0],20]))
            print('numBin:            ', numBin)
            plt.hist(dt,numBin)
            plt.xlabel('dt (sec)')
            plt.ylabel('count')
            plt.grid('on')
            plt.draw()

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
        #d = self.dev.getData()
        t = self.dev.getTime()
        self.timeList.append(t)
        self.count += 1
        self.outfile.write('{0} {1}\n'.format(self.count,t))
        print(self.count, t)


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
