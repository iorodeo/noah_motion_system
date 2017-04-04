#include "joy_trajectory.hpp"
#include "mctl/rawhid_msg_types.h"

#define _USE_MATH_DEFINES
#include <cmath>

JoyTrajectory::JoyTrajectory(Joystick *joystick)
{ 
    set_name("joy-spring");
    set_start_pos(get_pos(0.0));
    joystick_ptr_ = joystick;
}


double JoyTrajectory::get_joystick_force()
{
    std::cout << "joy" << std::endl;

    static const int32_t int16_max = std::numeric_limits<int16_t>::max();
    JoystickEvent event;

    if (joystick_ptr_ -> sample(&event))
    {
        if (event.isAxis() && !event.isInitialState())
        {
            if (event.number == joy_axis_)
            {
                joy_value_ = joy_const_*double(event.value)/int16_max;
            }
        }
    }
    return joy_value_;
}


void JoyTrajectory::update_state(double dt)
{
}


arma::Row<double> JoyTrajectory::get_pos(double t)
{ 
    arma::Row<double> pos(NumStepper,arma::fill::zeros);
    return pos;
}


arma::Row<double> JoyTrajectory::get_vel(double t)
{
    arma::Row<double> vel(NumStepper,arma::fill::zeros);
    return vel;
}


RtnStatus JoyTrajectory::next(DevToHostMsg msg, Configuration &config, TrajectoryData &data_next) 
{
    RtnStatus  rtn_status;

    double t = time(msg);

    data_next.pos = get_pos(t);
    data_next.vel = get_vel(t);

    std::cout << get_joystick_force() << std::endl;

    //set_done(true);


    return rtn_status;
}


