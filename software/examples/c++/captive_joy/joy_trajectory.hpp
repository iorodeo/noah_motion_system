#ifndef COS_TRAJECTORY_HPP
#define COS_TRAJECTORY_HPP
#include <mctl/mctl.hpp>
#include <mctl/joystick.hpp>

using namespace mctl;

class JoyTrajectory : public Trajectory
{
    public: 

        JoyTrajectory(Joystick *joystick); 

        void update_state(double dt);
        double get_joystick_force();

        arma::Row<double> get_pos(double t);
        arma::Row<double> get_vel(double t);

        virtual RtnStatus next(DevToHostMsg msg, Configuration &config, TrajectoryData &data_next) override;

    protected:

        Joystick *joystick_ptr_;
        int joy_axis_ = 1;
        double joy_const_ = -0.1;
        double joy_value_ = 0.0;

        Axis axis = Axis_X;
        double axis_pos_ = 0.0;
        double axis_vel_ = 0.0;

        double mass_ = 2.0;
        double spring_const_ = 0.1;

};


#endif
