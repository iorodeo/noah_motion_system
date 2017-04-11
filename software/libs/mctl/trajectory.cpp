#include "trajectory.hpp"
#include "msg_utility.hpp"
#include "rawhid_msg_types.h"

namespace mctl
{
    // Base class for captive trajectory

    Trajectory::Trajectory() 
    {
        pos_ = arma::Row<double>(NumStepper,arma::fill::zeros);
        vel_ = arma::Row<double>(NumStepper,arma::fill::zeros);
        start_pos_ = arma::Row<double>(NumStepper,arma::fill::zeros);
        dout_ = arma::Row<uint8_t>(NumDigitalOutput,arma::fill::zeros);
        done_ = true;
    }


    void Trajectory::initialize()
    {
        set_done(false);
        is_first_ = true;
    }


    void Trajectory::initialize(Configuration config)
    {
        pos_ = start_pos_;
        set_config(config);
        initialize();
    }


    Configuration Trajectory::config()
    {
        return config_;
    }


    void Trajectory::set_config(Configuration config)
    {
        config_ = config;
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


    arma::Row<double> Trajectory::start_position()
    {
        return start_pos_;
    }


    void Trajectory::set_start_position(arma::Row<double> start_pos)
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


    double Trajectory::message_period()
    {
        return 1.0e-6*double(MessagePeriod_us);
    }


    arma::Row<double> Trajectory::position()
    {
        return pos_;
    }


    void Trajectory::set_position(arma::Row<double> pos)
    {
        pos_ = pos;
    }


    arma::Row<double> Trajectory::velocity()
    {
        return vel_;
    }


    void Trajectory::set_velocity(arma::Row<double> vel)
    {
        vel_ = vel;
    }


    arma::Row<uint8_t> Trajectory::digital_output()
    {
        return dout_;
    }


    void Trajectory::set_digital_output(arma::Row<uint8_t> dout)
    {
        dout_ = dout;
    }


    RtnStatus Trajectory::update(DevToHostMsg msg)
    {
        RtnStatus rtn_status;
        set_position(start_pos_);
        set_done(true);
        return rtn_status;
    }


    arma::Row<double> Trajectory::force_and_torque(DevToHostMsg msg)
    {
        arma::Row<double> ft(ForceAndTorqueRowDim,arma::fill::zeros);
        arma::Row<double> ai = analog_input(msg);
        config_.get_force_and_torque(ai,ft);
        return ft;
    }


    arma::Row<double> Trajectory::analog_input(DevToHostMsg msg)
    {
        arma::Row<double> ai(NumAnalogInput,arma::fill::zeros);
        for (int i=0; i<NumAnalogInput; i++)
        {
            ai(i) = config_.analog_int_to_volt(msg.analog_input[i]);
        }
        return ai;
    }


} // namespace mctl
