#ifndef COS_TRAJECTORY_HPP
#define COS_TRAJECTORY_HPP
#include <mctl/mctl.hpp>

using namespace mctl;

class CosTrajectory : public Trajectory
{
    public: 

        CosTrajectory(); 

        arma::Row<double> get_pos(double t);
        arma::Row<double> get_vel(double t);

        virtual RtnStatus next(DevToHostMsg msg, Configuration &config, TrajectoryData &data_next) override;

    protected:

        double amplitude_ = 0.1;
        double period_ = 10.0;
        int num_cycles_ = 1;

        Axis axis = Axis_X;
};


#endif
