import scipy
import matplotlib.pyplot as plt

data = scipy.loadtxt('data.txt')
t = data[:,0]
xc = data[:,1]
xn = data[:,2]

plt.plot(t,xn,'r')
plt.plot(t,xc,'b')
plt.grid('on')
plt.show()
