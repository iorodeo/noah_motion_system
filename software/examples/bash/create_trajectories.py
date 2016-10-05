from __future__ import print_function
import scipy

def get_trajectory(amplitude, period, cycles, dt):
    num_pts = int(period*cycles/dt)
    trajectory = scipy.zeros((num_pts,amplitude.shape[0]))
    t = (scipy.arange(num_pts)*dt).transpose()
    for i,amp in enumerate(amplitude):
        trajectory[:,i] = amp*scipy.cos(2.0*scipy.pi*t/period)
    return t, trajectory

        
# -----------------------------------------------------------------------------
if __name__ == '__main__':

    amp_list = [ 
            scipy.array([0.1, 0.0 , 0.0, 0.0, 0.0]),
            scipy.array([0.0, 0.1 , 0.0, 0.0, 0.0]),
            scipy.array([0.0, 0.0 , 0.025 , 0.0, 0.0]),
            scipy.array([0.1, 0.1 , 0.025 , 0.0, 0.0]),
            ]

    cycles = 1
    period = 7.0
    dt = 0.005

    for i,amp in enumerate(amp_list):
        filename = 'trajectory{0}.txt'.format(i+1)
        print("creating: {0}".format(filename))

        t, traj = get_trajectory(amp,period,cycles,dt)
        scipy.savetxt(filename,traj)








