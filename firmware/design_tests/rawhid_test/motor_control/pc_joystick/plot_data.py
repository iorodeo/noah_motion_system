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
