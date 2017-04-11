#include "cos_trajectory.hpp"
#include "mctl/rawhid_msg_types.h"

#define _USE_MATH_DEFINES
#include <cmath>

CosTrajectory::CosTrajectory()
{ 
    set_name("cosine trajectory");

    // Set start position
    arma::Row<double> start_pos(NumStepper,arma::fill::zeros);
    start_pos(axis_) = get_cosine_position(0.0);
    set_start_position(start_pos);
}

double CosTrajectory::get_cosine_position(double t)
{ 
    return amplitude_*std::cos(2.0*M_PI*t/period_); 
}


double CosTrajectory::get_cosine_velocity(double t)
{
    return -((2.0*M_PI*amplitude_)/period_)*std::sin(2.0*M_PI*t/period_); 
}


RtnStatus CosTrajectory::update(DevToHostMsg msg) 
{
    RtnStatus  rtn_status;

    // Get current time and check to see if done?
    double t = time(msg);
    if (t >= period_*num_cycles_)
    {
        set_done(true);
    }

    // Get next motor positions and velocities
    arma::Row<double> pos_next(NumStepper,arma::fill::zeros);
    arma::Row<double> vel_next(NumStepper,arma::fill::zeros);
    pos_next(axis_) = get_cosine_position(t);
    vel_next(axis_) = get_cosine_velocity(t);

    // Set position and velocity
    set_position(pos_next);
    set_velocity(vel_next);

    return rtn_status;
}


