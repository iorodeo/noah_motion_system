"""
Copyright 2016 IO Rodeo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import scipy
import matplotlib.pyplot as plt

data = scipy.loadtxt('data.txt')
t = data[:,0]
f = data[:,1]
vs = data[:,2]
vm = data[:,3]
pc = data[:,4]
pn = data[:,5]
er = data[:,6]

plt.subplot(211)
plt.plot(t,pc,'b')
plt.plot(t,pn,'r')
plt.ylabel('position (steps)')
plt.grid('on')
#plt.title('captive trajectory - mass spring system')
plt.title('captive trajectory - mass w/ damping')
plt.subplot(212)
plt.plot(t,f,'g')
plt.grid('on')
plt.xlabel('t (sec)')
plt.ylabel('torque')
plt.show()

#plt.subplot(311)
#plt.plot(t,vs,'b')
#plt.plot(t,vm, 'r')
#plt.grid('on')
#plt.subplot(312)
#plt.plot(t,pc,'b')
#plt.plot(t,pn,'r')
#plt.grid('on')
#plt.subplot(313)
#plt.plot(t,er,'b')
#plt.grid('on')
#plt.show()
