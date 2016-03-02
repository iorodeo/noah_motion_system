#include "trajectory.hpp"
#include <cmath>
#include <iostream>

const double pi = std::acos(-1);

std::vector<TrajData> get_cosine_traj(double dt)
{
    std::vector<double> amplitude(constants::NumMotor); 
    double period = 10.0;

    for (int i=0; i<constants::NumMotor; i++)
    {
        amplitude[i] = 5000.0 - 500.0*i;
    }

    std::vector<TrajData> traj;
    double t = 0.0;

    while (t < period)
    {
        TrajData traj_data;
        for (int i=0; i<constants::NumMotor; i++)
        {
            traj_data.pos[i] = amplitude[i]*std::cos(2.0*pi*t/period);
            traj_data.vel[i] = -((2.0*pi*amplitude[i])/period)*std::sin(2.0*pi*t/period);
        }
        traj.push_back(traj_data);
        t += dt;
    }
    return traj;
}


std::vector<TrajData> get_multicosine_traj(double dt)
{
    std::vector<double> amplitude(constants::NumMotor); 
    double period = 10.0;

    for (int i=0; i<constants::NumMotor; i++)
    {
        amplitude[i] = 5000.0 - 500.0*i;
    }

    std::vector<TrajData> traj;
    double t = 0.0;

    while (t < period)
    {
        TrajData traj_data;
        for (int i=0; i<constants::NumMotor; i++)
        {
            traj_data.pos[i] =  amplitude[i]*std::cos(2.0*pi*t/period);
            traj_data.pos[i] += amplitude[i]*std::cos(6.0*pi*t/period);
            traj_data.vel[i] =  -((2.0*pi*amplitude[i])/period)*std::sin(2.0*pi*t/period);
            traj_data.vel[i] += -((6.0*pi*amplitude[i])/period)*std::sin(6.0*pi*t/period);
        }
        traj.push_back(traj_data);
        t += dt;
    }
    return traj;
}
