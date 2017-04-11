#ifndef COS_TRAJECTORY_HPP
#define COS_TRAJECTORY_HPP
#include <mctl/mctl.hpp>

using namespace mctl;

class CosTrajectory : public Trajectory
{
    public: 

        CosTrajectory(); 

        double get_cosine_position(double t);
        double get_cosine_velocity(double t);

        virtual RtnStatus update(DevToHostMsg msg) override;

    protected:

        Axis axis_ = Axis_X;
        double amplitude_ = 0.1;
        double period_ = 10.0;
        int num_cycles_ = 1;

};


#endif
