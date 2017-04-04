#include "cos_trajectory.hpp"
#include "mctl/rawhid_msg_types.h"

#define _USE_MATH_DEFINES
#include <cmath>

CosTrajectory::CosTrajectory()
{ 
    set_name("cosine");
    set_start_pos(get_pos(0.0));
}

arma::Row<double> CosTrajectory::get_pos(double t)
{ 
    arma::Row<double> pos(NumStepper,arma::fill::zeros);
    pos(axis) = amplitude_*std::cos(2.0*M_PI*t/period_); 
    return pos;
}


arma::Row<double> CosTrajectory::get_vel(double t)
{
    arma::Row<double> vel(NumStepper,arma::fill::zeros);
    vel(axis) = -((2.0*M_PI*amplitude_)/period_)*std::sin(2.0*M_PI*t/period_); 
    return vel;
}


RtnStatus CosTrajectory::next(DevToHostMsg msg, Configuration &config, TrajectoryData &data_next) 
{
    RtnStatus  rtn_status;

    double t = time(msg);

    data_next.pos = get_pos(t);
    data_next.vel = get_vel(t);

    if (t >= period_*num_cycles_)
    {
        set_done(true);
    }

    return rtn_status;
}


