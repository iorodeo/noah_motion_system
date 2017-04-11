#ifndef COS_TRAJECTORY_HPP
#define COS_TRAJECTORY_HPP
#include <mctl/mctl.hpp>
#include <mctl/joystick.hpp>

using namespace mctl;

class JoyTrajectory : public Trajectory
{

    public: 

        JoyTrajectory(Joystick &joystick); 

        virtual arma::Row<double> force_and_torque(DevToHostMsg msg) override;
        double joystick_value();

        virtual RtnStatus update(DevToHostMsg msg) override; 

    protected:

        Joystick *joystick_ptr_;
        bool done_ = false;

        int joy_axis_ = 1;
        double joy_value_ = 0.0;

        int ft_axis_ = 0;
        double ft_const_ = -0.01;

        Axis captive_axis_ = Axis_X;
        double captive_pos_ = 0.0;
        double captive_vel_ = 0.0;

        double mass_ = 5.0;
        double spring_const_ = 0.5;
        double spring_zero_ = 0.0;
        double damping_coef_ = 0.1;

};


#endif
