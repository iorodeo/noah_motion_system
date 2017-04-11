from __future__ import print_function
import h5py
import scipy
import matplotlib.pyplot as plt
import sys

filename = sys.argv[1]

motor_axis = 0
force_axis = 0

data = h5py.File(filename,'r')

t = data['time']
x = data['stepper_position'][:,motor_axis]
f = data['force_and_torque'][:,force_axis]

plt.plot(t,x)
plt.xlabel('t (s)')
plt.ylabel('position (m)')
plt.grid('on')

plt.show()





