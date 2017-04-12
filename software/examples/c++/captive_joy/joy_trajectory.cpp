#include "joy_trajectory.hpp"
#include "mctl/rawhid_msg_types.h"

#define _USE_MATH_DEFINES
#include <cmath>

JoyTrajectory::JoyTrajectory(Joystick &joystick)
{ 
    set_name("joy-spring");

    joystick_ptr_ = &joystick;

    // Set start position
    arma::Row<double> start_pos(NumStepper,arma::fill::zeros);
    start_pos(captive_axis_) = captive_pos_;
    set_start_position(start_pos);
}




RtnStatus JoyTrajectory::update(DevToHostMsg msg)
{
    RtnStatus  rtn_status;

    // Get time, time step, and force & torque vector from sensor
    double t = time(msg);
    double dt = message_period();
    arma::Row<double> ft_vector = force_and_torque(msg);

    // Extract relevant force axis and compute total force
    double force_sensor = ft_vector(ft_axis_);
    double force_spring = -spring_const_*(captive_pos_ - spring_zero_);
    double force_damping = -damping_coef_*captive_vel_;
    double force_total = force_sensor + force_spring + force_damping;

    // Integrate captive model (just Euler right now for simplicity)
    captive_vel_ += (dt/mass_)*force_total;
    captive_pos_ += dt*captive_vel_;

    // Create matrices for motor positon and velocity 
    arma::Row<double> pos_next(NumStepper,arma::fill::zeros);
    arma::Row<double> vel_next(NumStepper,arma::fill::zeros);

    // Set captive pos and vel for appropriate axis
    pos_next(captive_axis_) = captive_pos_;
    vel_next(captive_axis_) = captive_vel_;

    // Set next motor position and velocity
    set_position(pos_next);
    set_velocity(vel_next);

    // Check if done
    set_done(done_);

    return rtn_status;
}


arma::Row<double> JoyTrajectory::force_and_torque(DevToHostMsg msg) 
{
    arma::Row<double> ft(NumStepper,arma::fill::zeros);
    ft(ft_axis_) = ft_const_*joystick_value();
    return ft;
}


double JoyTrajectory::joystick_value()
{
    static const int32_t int16_max = std::numeric_limits<int16_t>::max();
    JoystickEvent event;

    if (joystick_ptr_ -> sample(&event))
    {
        if (event.isAxis() && !event.isInitialState())
        {
            if (event.number == joy_axis_)
            {
                joy_value_ = double(event.value)/int16_max;
            }
        }

        if (event.isButton() && !event.isInitialState())
        {
            done_ = true;
        }
    }
    return joy_value_;
}
