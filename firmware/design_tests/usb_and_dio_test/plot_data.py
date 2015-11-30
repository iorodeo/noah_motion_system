import scipy
import matplotlib.pyplot as plt


data = scipy.loadtxt('data.txt')
i = data[:,0]
t = 1.0e-6*data[:,1]

dt = t[1:] - t[:-1]

plt.plot(i[1:],dt,'.-')
plt.grid('on')
plt.xlabel('index')
plt.ylabel('dt (sec)')
plt.show()
