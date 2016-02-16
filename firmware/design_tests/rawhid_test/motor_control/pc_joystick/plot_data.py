import scipy
import matplotlib.pyplot as plt

data = scipy.loadtxt('data.txt')
t = data[:,0]
vs = data[:,1]
vm = data[:,2]
pc = data[:,3]
pn = data[:,4]
er = data[:,5]

plt.subplot(311)
plt.plot(t,vs,'b')
plt.plot(t,vm, 'r')
plt.grid('on')
plt.subplot(312)
plt.plot(t,pc,'b')
plt.plot(t,pn,'r')
plt.grid('on')
plt.subplot(313)
plt.plot(t,er,'b')
plt.grid('on')
plt.show()
