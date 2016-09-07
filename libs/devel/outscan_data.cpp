#include "outscan_data.hpp"

#include <H5Cpp.h>

namespace motion
{

    OutscanData::OutscanData() { }


    arma::Col<double> OutscanData::time()
    {
        arma::Col<double> time_vec = col_from_deque<double>(time_);
        time_vec = time_vec - time_vec[0];
        return time_vec;
    }


    arma::Mat<double> OutscanData::stepper_position()
    {
        return mat_from_deque<double>(stepper_position_,NumStepper);
    }


    arma::Mat<double> OutscanData::stepper_velocity()
    {
        return mat_from_deque<double>(stepper_velocity_,NumStepper);
    }


    arma::Mat<double> OutscanData::pwm_position()
    {
        return mat_from_deque<double>(pwm_position_,NumPwm);
    }


    arma::Mat<double> OutscanData::analog_input()
    {
        return mat_from_deque<double>(analog_input_,NumAnalogInput);
    }


    arma::Mat<double> OutscanData::force_and_torque()
    {
        return config_.get_force_and_torque(analog_input());
    }

    arma::Col<uint8_t> OutscanData::status()
    {
    }


    arma::Col<uint8_t> OutscanData::count()
    {
    }


    arma::Col<uint8_t> OutscanData::command()
    {
    }


    arma::Col<uint16_t> OutscanData::command_data() 
    {
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
        std::string units_str("units");

        H5::H5File h5file(filename,H5F_ACC_TRUNC);

        // Add time data to h5 file
        int  time_rank = 1;
        arma::Col<double> time_vec = time();
        hsize_t time_dims[] = {time_vec.size()};
        H5::DataSpace time_dataspace(time_rank,time_dims);
        H5::DataSet time_dataset = h5file.createDataSet("time", H5::PredType::NATIVE_DOUBLE, time_dataspace);
        time_dataset.write(time_vec.memptr(), H5::PredType::NATIVE_DOUBLE);

        std::string time_unit_str("sec");
        H5::DataSpace time_attr_dataspace(H5S_SCALAR);
        H5::StrType time_attr_type(H5::PredType::C_S1,time_unit_str.size());
        H5::Attribute time_attr = time_dataset.createAttribute(units_str,time_attr_type,time_attr_dataspace); 
        time_attr.write(time_attr_type,time_unit_str);

        // Note arma::Mat is column major, but hdf5 expects row major so we work with transpose 
        // and swap column and row dimensions.  
        
        // Add stepper position to h5 file. 
        int position_rank = 2;
        arma::Mat<double> position_mat = stepper_position_t();
        hsize_t position_dims[] = {position_mat.n_cols, position_mat.n_rows};
        H5::DataSpace position_dataspace(position_rank, position_dims);
        H5::DataSet position_dataset = h5file.createDataSet("stepper_position", H5::PredType::NATIVE_DOUBLE,position_dataspace); 
        position_dataset.write(position_mat.memptr(), H5::PredType::NATIVE_DOUBLE);

        // Add stepper velocity
        int velocity_rank = 2;
        arma::Mat<double> velocity_mat = stepper_velocity_t();
        hsize_t velocity_dims[] = {velocity_mat.n_cols, velocity_mat.n_rows};
        H5::DataSpace velocity_dataspace(velocity_rank, velocity_dims);
        H5::DataSet velocity_dataset = h5file.createDataSet("stepper_velocity", H5::PredType::NATIVE_DOUBLE,velocity_dataspace); 
        velocity_dataset.write(velocity_mat.memptr(), H5::PredType::NATIVE_DOUBLE);


        return rtn_status;
    }


    // OutscanData protected methods
    // ----------------------------------------------------------------------------------
    arma::Mat<double> OutscanData::stepper_position_t()
    {
        return mat_trans_from_deque<double>(stepper_position_, NumStepper);
    }


    arma::Mat<double> OutscanData::stepper_velocity_t()
    {
        return mat_trans_from_deque<double>(stepper_velocity_, NumStepper);
    }


    arma::Mat<double> OutscanData::pwm_position_t()
    {
        return mat_trans_from_deque<double>(pwm_position_, NumPwm);
    }


    arma::Mat<double> OutscanData::analog_input_t()
    {
        return mat_trans_from_deque<double>(analog_input_,NumAnalogInput);
    }


    arma::Mat<double> OutscanData::force_and_torque_t()
    {
        arma::Mat<double> ft_mat = force_and_torque();
        return ft_mat.t();
    }


} // namespace motion

