#include "outscan_data.hpp"

#include <algorithm>

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
        return col_from_deque<uint8_t>(status_);
    }


    arma::Col<uint8_t> OutscanData::count()
    {
        return col_from_deque<uint8_t>(count_);
    }


    arma::Col<uint8_t> OutscanData::command()
    {
        return col_from_deque<uint8_t>(command_);
    }


    arma::Col<uint16_t> OutscanData::command_data() 
    {
        return col_from_deque<uint16_t>(command_data_);
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

        // TODO: add status checks 
        // -----------------------------------------------------

        rtn_status = add_time_dataset(h5file);

        rtn_status = add_stepper_position_dataset(h5file);

        rtn_status = add_stepper_velocity_dataset(h5file);

        rtn_status = add_pwm_position_dataset(h5file);

        rtn_status = add_analog_input_dataset(h5file);

        rtn_status = add_force_and_torque_dataset(h5file);

        rtn_status = add_status_dataset(h5file);

        rtn_status = add_count_dataset(h5file);

        rtn_status = add_command_dataset(h5file);

        rtn_status = add_command_data_dataset(h5file);

        // -----------------------------------------------------

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


    RtnStatus OutscanData::add_time_dataset(H5::H5File &h5file)
    {
        RtnStatus rtn_status;

        // Add array data
        int  rank = 1;
        arma::Col<double> time_vec = time();
        hsize_t dims[] = {time_vec.size()};
        H5::DataSpace dataspace(rank,dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
        H5::DataSet dataset = h5file.createDataSet("time",datatype,dataspace);
        dataset.write(time_vec.memptr(),datatype);

        // Add units attribute
        std::string units_str("sec");
        H5::DataSpace units_dataspace(H5S_SCALAR);
        H5::StrType units_type(H5::PredType::C_S1, units_str.size());
        H5::Attribute units_attr = dataset.createAttribute(units_name_, units_type, units_dataspace); 
        units_attr.write(units_type,units_str);
        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_position_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;

        // Add matrix data
        int rank = 2;
        arma::Mat<double> pos_mat = stepper_position_t();
        hsize_t dims[] = {pos_mat.n_cols, pos_mat.n_rows};
        H5::DataSpace dataspace(rank,dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
        H5::DataSet dataset = h5file.createDataSet("stepper_position",datatype,dataspace); 
        dataset.write(pos_mat.memptr(),datatype);
        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_velocity_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;

        // Add matrix data
        int rank = 2;
        arma::Mat<double> vel_mat = stepper_velocity_t();
        hsize_t dims[] = {vel_mat.n_cols, vel_mat.n_rows};
        H5::DataSpace dataspace(rank, dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
        H5::DataSet dataset = h5file.createDataSet("stepper_velocity",datatype,dataspace); 
        dataset.write(vel_mat.memptr(),datatype);
        return rtn_status;
    }

    RtnStatus OutscanData::add_pwm_position_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;

        // Add matrix data
        int rank = 2;
        arma::Mat<double> pwm_mat = pwm_position_t();
        hsize_t dims[] = {pwm_mat.n_cols, pwm_mat.n_rows};
        H5::DataSpace dataspace(rank, dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
        H5::DataSet dataset = h5file.createDataSet("pwm_velocity",datatype,dataspace); 
        dataset.write(pwm_mat.memptr(),datatype);
        return rtn_status;
    }

    RtnStatus OutscanData::add_analog_input_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;

        // Add matrix data
        int rank = 2;
        arma::Mat<double> ain_mat = analog_input_t();
        hsize_t dims[] = {ain_mat.n_cols, ain_mat.n_rows};
        H5::DataSpace dataspace(rank, dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
        H5::DataSet dataset = h5file.createDataSet("analog_input",datatype,dataspace); 
        dataset.write(ain_mat.memptr(),datatype);
        return rtn_status;
    }

    RtnStatus OutscanData::add_force_and_torque_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;

        // Add matrix data
        int rank = 2;
        arma::Mat<double> ft_mat = force_and_torque_t();
        hsize_t dims[] = {ft_mat.n_cols, ft_mat.n_rows};
        H5::DataSpace dataspace(rank, dims);
        H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
        H5::DataSet dataset = h5file.createDataSet("force_and_torque",datatype,dataspace); 
        dataset.write(ft_mat.memptr(),datatype);
        return rtn_status;
    }

    RtnStatus OutscanData::add_status_dataset(H5::H5File &h5file)
    {
        RtnStatus rtn_status;

        // Add status data vector
        int  rank = 1;
        arma::Col<uint8_t> status_vec = status();
        hsize_t dims[] = {status_vec.size()};
        H5::DataSpace dataspace(rank,dims);
        H5::DataType datatype = H5::PredType::NATIVE_UINT8;
        H5::DataSet dataset = h5file.createDataSet("status",datatype,dataspace);
        dataset.write(status_vec.memptr(),datatype);
        return rtn_status;
    }


    RtnStatus OutscanData::add_count_dataset(H5::H5File &h5file)
    {
        RtnStatus rtn_status;

        // Add count data vector
        int  rank = 1;
        arma::Col<uint8_t> count_vec = count();
        hsize_t dims[] = {count_vec.size()};
        H5::DataSpace dataspace(rank,dims);
        H5::DataType datatype = H5::PredType::NATIVE_UINT8;
        H5::DataSet dataset = h5file.createDataSet("count",datatype,dataspace);
        dataset.write(count_vec.memptr(),datatype);
        return rtn_status;
    }


    RtnStatus OutscanData::add_command_dataset(H5::H5File &h5file)
    {
        RtnStatus rtn_status;

        // Add command data vector
        int  rank = 1;
        arma::Col<uint8_t> cmd_vec = command();
        hsize_t dims[] = {cmd_vec.size()};
        H5::DataSpace dataspace(rank,dims);
        H5::DataType datatype = H5::PredType::NATIVE_UINT8;
        H5::DataSet dataset = h5file.createDataSet("command",datatype,dataspace);
        dataset.write(cmd_vec.memptr(),datatype);

        return rtn_status;
    }


    RtnStatus OutscanData::add_command_data_dataset(H5::H5File &h5file)
    {
        RtnStatus rtn_status;

        // Add command data vector
        int  rank = 1;
        arma::Col<uint16_t> cmd_data_vec = command_data();
        hsize_t dims[] = {cmd_data_vec.size()};
        H5::DataSpace dataspace(rank,dims);
        H5::DataType datatype = H5::PredType::NATIVE_UINT16;
        H5::DataSet dataset = h5file.createDataSet("command_data",datatype,dataspace);
        dataset.write(cmd_data_vec.memptr(),datatype);
        return rtn_status;
    }

} // namespace motion

