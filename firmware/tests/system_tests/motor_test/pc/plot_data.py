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
import sys
import scipy
import matplotlib.pyplot as plt

data = scipy.loadtxt(sys.argv[1])
t = data[:,0]

if data.shape[1] == 2:

    x = data[:,1]
    plt.plot(t,x)
    plt.grid('on')

else:

    num_plot = (data.shape[1]-1)/2
    for i in range(num_plot):
        n = 2*i + 1
        m = 2*i + 2
        p = data[:,n]
        q = data[:,m]
        plt.plot(t,p,'b')
        plt.plot(t,q,'r')
        plt.grid('on')

plt.xlabel('time (sec)')
plt.show()
