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
xc = data[:,1]
xn = data[:,2]

plt.plot(t,xn,'r')
plt.plot(t,xc,'b')
plt.grid('on')
plt.show()
