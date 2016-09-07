#include "outscan_data.hpp"

#include <H5Cpp.h>

namespace motion
{

    OutscanData::OutscanData() { }

    arma::Mat<double> OutscanData::time()
    {
        arma::Mat<double> time_mat(time_.size(),1); 
        int ind = 0;
        double t0 = time_[0];
        for (auto val : time_)
        {
            time_mat(ind,0) = val - t0;;
            ind++;
        }
        return time_mat;
    }


    void OutscanData::update(DevToHostMsg msg)
    {
        // Add time in seconds
        time_.push_back(1.0e-6*double(msg.time_us));

        // Add stepper positions and velocities
        std::vector<double> pos_vec(NumStepper);
        std::vector<double> vel_vec(NumStepper);
        for (int i=0; i<NumStepper;i++)
        {
            pos_vec[i] = config_.index_to_unit(Axis(i), msg.stepper_position[i]);
            vel_vec[i] = config_.index_to_unit(Axis(i), msg.stepper_velocity[i]);
        }
        stepper_position_.push_back(pos_vec);
        stepper_velocity_.push_back(vel_vec);

        // Add pwm positions
        std::vector<double> pwm_vec(NumPwm);
        int i = 0;
        for (auto axis : PwmList)
        {
            pwm_vec[i] = config_.index_to_unit(Axis(axis), msg.pwm_position[i]);
            i++;
        }
        pwm_position_.push_back(pwm_vec);

        // Add analog input
        std::vector<double> analog_vec(NumAnalogInput);
        for (int i=0; i<NumAnalogInput;i++)
        {
            analog_vec[i] = config_.analog_int_to_volt(msg.analog_input[i]);
        }
        analog_input_.push_back(analog_vec);

        // Add device information - status, count, command, command_data. 
        status_.push_back(msg.status);
        count_.push_back(msg.count);
        command_.push_back(msg.command);
        command_data_.push_back(msg.command_data);
    }
    

    void OutscanData::clear()
    {
        time_.clear();
        stepper_position_.clear();
        stepper_velocity_.clear();
        pwm_position_.clear();
        analog_input_.clear();
        status_.clear();
        count_.clear();
        command_.clear();
        command_data_.clear();
    }


    RtnStatus OutscanData::save(std::string filename)
    {
        RtnStatus rtn_status;

        H5::H5File h5file(filename,H5F_ACC_TRUNC);

        // Add time data to h5 file
        int  time_rank = 1;
        hsize_t time_dims[] = {time_.size()};
        H5::DataSpace time_dataspace(time_rank,time_dims);
        
        H5::DataSet dataset = h5file.createDataSet("time",H5::PredType::NATIVE_DOUBLE,time_dataspace);
        std::vector<double> time_vec(time_.begin(), time_.end());
        dataset.write(time_vec.data(),H5::PredType::NATIVE_DOUBLE);


        return rtn_status;
    }
}

