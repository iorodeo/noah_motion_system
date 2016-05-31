from __future__ import print_function
import serial
import time
import matplotlib.pyplot as plt

class AinReader(serial.Serial):

    params = {'baudrate': 115200, 'timeout': 5000}

    def __init__(self,port='/dev/ttyACM0'):
        super(AinReader,self).__init__(port,**self.params)
        time.sleep(2.0)

    def read_data(self,num_sample=100):
        sample_count = 0
        self.write('r\n')
        data_list = []
        while (sample_count < num_sample):
            data = self.readline()
            data = data.strip()
            try:
                data_list.append(float(data))
            except:
                continue
            sample_count += 1
            print(sample_count, float(data))
        self.write('s\n')
        return data_list






# ---------------------------------------------------------------------------------------
if __name__ == '__main__':

    dev = AinReader()
    data_list = dev.read_data(5000)
    plt.plot(data_list,'-')
    plt.show()

