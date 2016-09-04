import scipy
import matplotlib.pyplot as plt

def get_trajectory(amplitude, period, cycles, dt):
    num_pts = int(period*cycles/dt)
    trajectory = scipy.zeros((num_pts,amplitude.shape[0]))
    t = (scipy.arange(num_pts)*dt).transpose()
    for i,amp in enumerate(amplitude):
        trajectory[:,i] = amp*scipy.cos(2.0*scipy.pi*t/period)
    return t, trajectory

        
# -----------------------------------------------------------------------------
if __name__ == '__main__':

    filename = 'test_trajectory.txt'
    amplitude = scipy.array([0.05, 0.03 , 0.015 , 20.0, 0.0])
    cycles = 4
    period = 5.0
    dt = 0.005

    t, traj = get_trajectory(amplitude,period,cycles,dt)
    scipy.savetxt(filename,traj)

    for i in range(traj.shape[1]):
        plt.plot(t, traj)
    plt.show()








