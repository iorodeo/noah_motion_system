#include "outscan_data.hpp"

#include <H5Cpp.h>

namespace motion
{

    OutscanData::OutscanData() { }

    arma::Col<double> OutscanData::time()
    {
        arma::Col<double> time_vec(time_.size()); 
        int ind = 0;
        double t0 = time_[0];
        for (auto val : time_)
        {
            time_vec(ind) = val - t0;;
            ind++;
        }
        return time_vec;
    }

    arma::Mat<double> OutscanData::stepper_position()
    {
        arma::Mat<double> pos_mat;
        if (stepper_position_.size() > 0)
        {
            int n_rows = stepper_position_.size();
            int n_cols = stepper_position_[0].size();
            if (n_cols == NumStepper)
            {
                pos_mat.resize(n_rows,n_cols);
                int ind = 0;
                for (auto pos_vec : stepper_position_)
                {
                    if (pos_vec.size() == NumStepper)
                    {
                        pos_mat.row(ind) = arma::Row<double>(pos_vec);
                    }
                    else
                    {
                        pos_mat.zeros();
                        break;
                    }
                    ind ++;
                }
            }
        }
        return pos_mat;
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
        arma::Col<double> time_vec = time();
        hsize_t time_dims[] = {time_vec.size()};
        H5::DataSpace time_dataspace(time_rank,time_dims);
        H5::DataSet time_dataset = h5file.createDataSet("time",H5::PredType::NATIVE_DOUBLE,time_dataspace);
        time_dataset.write(time_vec.memptr(), H5::PredType::NATIVE_DOUBLE);

        // Add stepper position to h5 file. Note arma::Mat is column major, but hdf5 expects row major so
        // we work with transpose and swap column and row dimensions.
        int position_rank = 2;
        arma::Mat<double> position_mat = stepper_position().t();
        hsize_t position_dims[] = {position_mat.n_cols, position_mat.n_rows};
        H5::DataSpace position_dataspace(position_rank, position_dims);
        H5::DataSet position_dataset = h5file.createDataSet("stepper_position", H5::PredType::NATIVE_DOUBLE,position_dataspace); 
        position_dataset.write(position_mat.memptr(), H5::PredType::NATIVE_DOUBLE);



        return rtn_status;
    }
}

