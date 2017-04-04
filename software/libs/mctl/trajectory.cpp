#include "trajectory.hpp"
#include "rawhid_msg_types.h"

namespace mctl
{
    // Base class for captive trajectory

    Trajectory::Trajectory() 
    {
        start_pos_ = arma::Row<double>(NumStepper,arma::fill::zeros);
        done_ = true;
    }

    void Trajectory::initialize()
    {
        set_done(false);
        is_first_ = true;
    }


    bool Trajectory::done()
    {
        return done_; 
    }


    void Trajectory::set_done(bool done)
    {
        done_ = done;
    }


    std::string Trajectory::name()
    {
        return name_;
    }


    void Trajectory::set_name(std::string name)
    {
        name_ = name;
    }


    arma::Row<double> Trajectory::start_pos()
    {
        return start_pos_;
    }


    void Trajectory::set_start_pos(arma::Row<double> start_pos)
    {
        start_pos_ = start_pos;
    }

    double Trajectory::time(DevToHostMsg msg)
    {
        if (is_first_)
        {
            start_time_us_ = msg.time_us;
            is_first_ = false;
        }
        return  1.0e-6*double(msg.time_us - start_time_us_);
    }


    RtnStatus Trajectory::next(DevToHostMsg msg, Configuration &config, TrajectoryData &data_next)
    {
        RtnStatus rtn_status;
        set_done(true);
        return rtn_status;
    }


} // namespace mctl
