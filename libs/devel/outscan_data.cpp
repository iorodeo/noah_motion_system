#include "outscan_data.hpp"

#include <H5Cpp.h>

namespace motion
{

    OutscanData::OutscanData() 
    {
        for (int i=0; i<100; i++)
        {
            time_.push_back(0.1*i);
        }
    }

    arma::Mat<double> OutscanData::time()
    {
        arma::Mat<double> time_mat(time_.size(),1); 
        int ind = 0;
        for (auto val : time_)
        {
            time_mat(ind,0) = val;
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
        
    }


    RtnStatus OutscanData::save(std::string filename)
    {
        RtnStatus rtn_status;

        H5::H5File h5file(filename,H5F_ACC_TRUNC);
        //H5::DataSet dataset = h5file.openDataSet(std::string("outscan_data"));


        return rtn_status;
    }
}

        //uint64_t time_us;
        //int16_t  stepper_position[NumStepper];
        //int16_t  stepper_velocity[NumStepper];
        //uint16_t pwm_position[NumPwm];
        //uint16_t analog_input[NumAnalogInput];
        //uint8_t  status;  
        //uint8_t  count;
        //uint8_t  command;
        //uint16_t command_data;
        //uint8_t  padding;
