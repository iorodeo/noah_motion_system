#include "outscan_data.hpp"
#include "rawhid_msg_types.h"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <H5Cpp.h>

namespace mctl
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
        arma::Mat<double> ft_mat;
        arma::Mat<double> ain_mat = analog_input();
        RtnStatus rtn_status = config_.get_force_and_torque(ain_mat,ft_mat);
        return ft_mat;
    }


    arma::Mat<double> OutscanData::stepper_position_cmd()
    {
        return mat_from_deque<double>(stepper_position_cmd_,NumStepper);
    }


    arma::Mat<double> OutscanData::stepper_velocity_cmd()
    {
        return mat_from_deque<double>(stepper_velocity_cmd_,NumStepper);
    }


    arma::Mat<uint8_t> OutscanData::dio_cmd()
    {
        return mat_from_deque<uint8_t>(dio_cmd_,NumDigitalOutput);
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

    Configuration OutscanData::config()
    {
        return config_;
    }

    void OutscanData::set_config(Configuration new_config)
    {
        config_ = new_config;
    }


    void OutscanData::update(DevToHostMsg msg, CommandData cmd)
    {
        // Add time in seconds
        time_.push_back(1.0e-6*double(msg.time_us));

        // Add stepper positions and velocities
        std::vector<double> pos_vec(NumStepper);
        std::vector<double> vel_vec(NumStepper);
        std::vector<double> pos_cmd_vec(NumStepper);
        std::vector<double> vel_cmd_vec(NumStepper);
        for (int i=0; i<NumStepper;i++)
        {
            pos_vec[i] = config_.index_to_unit(Axis(i), msg.stepper_position[i]);
            vel_vec[i] = config_.index_to_unit(Axis(i), msg.stepper_velocity[i]);
            pos_cmd_vec[i] = config_.index_to_unit(Axis(i), cmd.stepper_position[i]);
            vel_cmd_vec[i] = config_.index_to_unit(Axis(i), cmd.stepper_velocity[i]);
        }
        stepper_position_.push_back(pos_vec);
        stepper_velocity_.push_back(vel_vec);
        stepper_position_cmd_.push_back(pos_cmd_vec);
        stepper_velocity_cmd_.push_back(vel_cmd_vec);

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

        // Add DIO commands
        std::vector<uint8_t> dio_vec = arma::conv_to<std::vector<uint8_t>>::from(cmd.dio);
        dio_cmd_.push_back(dio_vec);

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
        H5::H5File h5file;

        rtn_status = open_h5file(h5file,filename);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_date_attribute(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_time_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_stepper_position_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_stepper_velocity_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_pwm_position_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_analog_input_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_force_and_torque_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_stepper_position_cmd_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_stepper_velocity_cmd_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_dio_cmd_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_status_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_count_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_command_dataset(h5file);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_command_data_dataset(h5file);
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


    arma::Mat<double> OutscanData::stepper_position_cmd_t()
    {
        return mat_trans_from_deque<double>(stepper_position_cmd_, NumStepper);
    }


    arma::Mat<double> OutscanData::stepper_velocity_cmd_t()
    {
        return mat_trans_from_deque<double>(stepper_velocity_cmd_, NumStepper);
    }


    arma::Mat<uint8_t> OutscanData::dio_cmd_t()
    {
        return mat_trans_from_deque<uint8_t>(dio_cmd_, NumDigitalOutput);
    }


    RtnStatus OutscanData::open_h5file(H5::H5File &h5file, std::string filename)
    {
        RtnStatus rtn_status;
        try
        {
            h5file = H5::H5File(filename,H5F_ACC_TRUNC);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }
        return rtn_status;
    }


    RtnStatus OutscanData::add_time_dataset(H5::H5File &h5file)
    {
        RtnStatus rtn_status;
        std::string dataset_name("time");
        arma::Col<double> time_vec = time();

        rtn_status = add_col_double_dataset(h5file,time_vec,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        std::string unit_str("sec");
        rtn_status = add_dataset_attribute(h5file,dataset_name,unit_attr_name_,unit_str);

        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_position_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;
        std::string dataset_name("stepper_position");
        arma::Mat<double> pos_mat = stepper_position_t();

        rtn_status = add_mat_double_dataset(h5file,pos_mat,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_stepper_position_unit_attribute(h5file,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_stepper_axis_attribute(h5file,dataset_name);

        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_velocity_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;
        std::string dataset_name("stepper_velocity");
        arma::Mat<double> vel_mat = stepper_velocity_t();

        rtn_status = add_mat_double_dataset(h5file,vel_mat,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_stepper_velocity_unit_attribute(h5file,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_stepper_axis_attribute(h5file,dataset_name);

        return rtn_status;
    }


    RtnStatus OutscanData::add_pwm_position_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;
        std::string dataset_name("pwm_position");
        arma::Mat<double> pwm_mat = pwm_position_t();

        rtn_status = add_mat_double_dataset(h5file,pwm_mat,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_pwm_unit_attribute(h5file,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_pwm_axis_attribute(h5file,dataset_name);

        return rtn_status;
    }


    RtnStatus OutscanData::add_analog_input_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;
        std::string dataset_name("analog_input");
        arma::Mat<double> ain_mat = analog_input_t();

        rtn_status = add_mat_double_dataset(h5file,ain_mat,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_analog_input_unit_attribute(h5file,dataset_name);
        return rtn_status;
    }


    RtnStatus OutscanData::add_force_and_torque_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;
        std::string dataset_name("force_and_torque");
        arma::Mat<double> ft_mat = force_and_torque_t();

        rtn_status = add_mat_double_dataset(h5file,ft_mat,dataset_name);

        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_force_and_torque_attribute(h5file,dataset_name);
        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_position_cmd_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;
        std::string dataset_name("stepper_position_cmd");
        arma::Mat<double> pos_cmd_mat = stepper_position_cmd_t();

        rtn_status = add_mat_double_dataset(h5file,pos_cmd_mat,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = add_stepper_position_unit_attribute(h5file,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_stepper_axis_attribute(h5file,dataset_name);

        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_velocity_cmd_dataset(H5::H5File &h5file)
    {
        // Note, arma matrices are stored in column major order whereas hdf5(C++) expects 
        // matrices in row major order - so we work wiht the transpose matrix.
        RtnStatus rtn_status;
        std::string dataset_name("stepper_velocity_cmd");
        arma::Mat<double> vel_cmd_mat = stepper_velocity_cmd_t();

        rtn_status = add_mat_double_dataset(h5file,vel_cmd_mat,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_stepper_velocity_unit_attribute(h5file,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = add_stepper_axis_attribute(h5file,dataset_name);

        return rtn_status;
    }


    RtnStatus OutscanData::add_dio_cmd_dataset(H5::H5File &h5file)
    {
        RtnStatus rtn_status;
        std::string dataset_name("dio_cmd");
        arma::Mat<uint8_t> dio_cmd_mat = dio_cmd_t();
        rtn_status = add_mat_uint8_dataset(h5file,dio_cmd_mat,dataset_name);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        return rtn_status;
    }


    RtnStatus OutscanData::add_status_dataset(H5::H5File &h5file)
    {
        std::string dataset_name("status");
        arma::Col<uint8_t> status_vec = status();
        return add_col_uint8_dataset(h5file,status_vec,dataset_name);
    }


    RtnStatus OutscanData::add_count_dataset(H5::H5File &h5file)
    {
        std::string dataset_name("count");
        arma::Col<uint8_t> count_vec = count();
        return add_col_uint8_dataset(h5file,count_vec,dataset_name);
    }


    RtnStatus OutscanData::add_command_dataset(H5::H5File &h5file)
    {
        std::string dataset_name("command");
        arma::Col<uint8_t> cmd_vec = command();
        return add_col_uint8_dataset(h5file,cmd_vec,dataset_name);
    }


    RtnStatus OutscanData::add_command_data_dataset(H5::H5File &h5file)
    {
        std::string dataset_name("command_data");
        arma::Col<uint16_t> cmd_data_vec = command_data();
        return add_col_uint16_dataset(h5file,cmd_data_vec,dataset_name);
    }


    RtnStatus OutscanData::add_date_attribute(H5::H5File &h5file)
    {
        RtnStatus rtn_status;
        std::string group_name("/");

        std::time_t now = std::time(nullptr);
        std::ostringstream date_data_oss;
        date_data_oss << std::asctime(std::localtime(&now));

        rtn_status = add_group_attribute(h5file,group_name,date_attr_name_,date_data_oss.str());
        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_position_unit_attribute(H5::H5File &h5file, std::string dataset_name)
    {
        RtnStatus rtn_status;
        std::ostringstream unit_oss;
        for (int i=0; i<NumStepper; i++)
        {
            unit_oss << config_.axis_unit_string(Axis(i));
            if (i < (NumStepper-1))
            {
                unit_oss << ", ";
            }
        }
        rtn_status = add_dataset_attribute(h5file,dataset_name,unit_attr_name_,unit_oss.str());
        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_velocity_unit_attribute(H5::H5File &h5file, std::string dataset_name)
    {
        RtnStatus rtn_status;
        std::ostringstream unit_oss;
        for (int i=0; i<NumStepper; i++)
        {
            unit_oss << config_.axis_unit_string(Axis(i)) << std::string("/s");
            if (i < (NumStepper-1))
            {
                unit_oss << ", ";
            }
        }
        rtn_status = add_dataset_attribute(h5file,dataset_name,unit_attr_name_,unit_oss.str());
        return rtn_status;
    }


    RtnStatus OutscanData::add_stepper_axis_attribute(H5::H5File &h5file, std::string dataset_name)
    {
        RtnStatus rtn_status;
        std::ostringstream axis_oss;
        for (int i=0; i<NumStepper; i++)
        {
            axis_oss << config_.axis_name(Axis(i));
            if (i < (NumStepper-1))
            {
                axis_oss << ", ";
            }
        }
        rtn_status = add_dataset_attribute(h5file, dataset_name, axis_attr_name_, axis_oss.str());
        return rtn_status;
    }

    RtnStatus OutscanData::add_pwm_unit_attribute(H5::H5File &h5file, std::string dataset_name)
    {
        RtnStatus rtn_status;
        std::ostringstream unit_oss;
        int ind = 0;
        for (auto num : PwmList)
        {
            unit_oss << config_.axis_unit_string(Axis(num));
            if (ind < (NumPwm-1))
            {
                unit_oss << ", ";
            }
        }
        rtn_status = add_dataset_attribute(h5file, dataset_name, unit_attr_name_, unit_oss.str());
        return rtn_status;
    }
    

    RtnStatus OutscanData::add_pwm_axis_attribute(H5::H5File &h5file, std::string dataset_name)
    {
        RtnStatus rtn_status;
        std::ostringstream axis_oss;
        int ind = 0;
        for (auto num : PwmList)
        {
            axis_oss << config_.axis_name(Axis(num));
            if (ind < (NumPwm-1))
            {
                axis_oss << ", ";
            }
            ind++;
        }
        rtn_status = add_dataset_attribute(h5file, dataset_name, axis_attr_name_, axis_oss.str());
        return rtn_status;
    }


    RtnStatus OutscanData::add_analog_input_unit_attribute(H5::H5File &h5file, std::string dataset_name)
    {
        RtnStatus rtn_status;
        std::ostringstream ain_oss;
        for (int i=0; i<NumAnalogInput; i++)
        {
            ain_oss << config_.analog_input_unit_string();
            if (i < (NumAnalogInput-1))
            {
                ain_oss << ", ";
            }
        }
        rtn_status = add_dataset_attribute(h5file, dataset_name, unit_attr_name_, ain_oss.str());
        return rtn_status;
    }


    RtnStatus OutscanData::add_force_and_torque_attribute(H5::H5File &h5file, std::string dataset_name)
    {
        RtnStatus rtn_status;
        std::vector<std::string> units_vec;
        rtn_status = config_.get_force_and_torque_units(units_vec);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        std::ostringstream ft_oss;
        for (int i=0; i<units_vec.size(); i++)
        {
            ft_oss << units_vec[i];
            if (i < units_vec.size()-1)
            {
                ft_oss << ", ";
            }
        }
        rtn_status = add_dataset_attribute(h5file, dataset_name, unit_attr_name_, ft_oss.str());
        return rtn_status;
    }


    RtnStatus OutscanData::add_col_uint8_dataset(H5::H5File &h5file, arma::Col<uint8_t> &col, std::string name)
    {
        RtnStatus rtn_status;
        int rank = 1;
        hsize_t dims[] = {col.size()};
        try
        {
            H5::DataSpace dataspace(rank,dims);
            H5::DataType datatype = H5::PredType::NATIVE_UINT8;
            H5::DataSet dataset = h5file.createDataSet(name,datatype,dataspace); 
            dataset.write(col.memptr(),datatype);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }

        return rtn_status;
    }


    RtnStatus OutscanData::add_col_uint16_dataset(H5::H5File &h5file, arma::Col<uint16_t> &col, std::string name)
    {
        RtnStatus rtn_status;
        int rank = 1;
        hsize_t dims[] = {col.size()};
        try
        {
            H5::DataSpace dataspace(rank,dims);
            H5::DataType datatype = H5::PredType::NATIVE_UINT16;
            H5::DataSet dataset = h5file.createDataSet(name,datatype,dataspace); 
            dataset.write(col.memptr(),datatype);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }
        return rtn_status;
    }


    RtnStatus OutscanData::add_col_double_dataset(H5::H5File &h5file, arma::Col<double> &col, std::string name)
    {
        RtnStatus rtn_status;
        int rank = 1;
        hsize_t dims[] = {col.size()};

        try
        {
            H5::DataSpace dataspace(rank,dims);
            H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
            H5::DataSet dataset = h5file.createDataSet(name,datatype,dataspace); 
            dataset.write(col.memptr(),datatype);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }
        return rtn_status;
    }


    RtnStatus OutscanData::add_mat_uint8_dataset(H5::H5File &h5file, arma::Mat<uint8_t> &mat_t, std::string name)
    {
        // ---------------------------------------------------------------------------------------------------------
        // Note, mat_t is matrix transpose. This is due to fact that Armadillo stores matrices in column major format
        // whereas HDF5 expects the matrix to be in row major format.
        // ----------------------------------------------------------------------------------------------------------
        RtnStatus rtn_status;
        int rank = 2;
        hsize_t dims[] = {mat_t.n_cols, mat_t.n_rows};

        try
        {
            H5::DataSpace dataspace(rank,dims);
            H5::DataType datatype = H5::PredType::NATIVE_UINT8;
            H5::DataSet dataset = h5file.createDataSet(name,datatype,dataspace); 
            dataset.write(mat_t.memptr(),datatype);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }
        return rtn_status;
    }


    RtnStatus OutscanData::add_mat_double_dataset(H5::H5File &h5file, arma::Mat<double> &mat_t, std::string name)
    {
        // ---------------------------------------------------------------------------------------------------------
        // Note, mat_t is matrix transpose. This is due to fact that Armadillo stores matrices in column major format
        // whereas HDF5 expects the matrix to be in row major format.
        // ----------------------------------------------------------------------------------------------------------
        RtnStatus rtn_status;
        int rank = 2;
        hsize_t dims[] = {mat_t.n_cols, mat_t.n_rows};

        try
        {
            H5::DataSpace dataspace(rank,dims);
            H5::DataType datatype = H5::PredType::NATIVE_DOUBLE;
            H5::DataSet dataset = h5file.createDataSet(name,datatype,dataspace); 
            dataset.write(mat_t.memptr(),datatype);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }
        return rtn_status;
    }


    RtnStatus OutscanData::add_group_attribute(H5::H5File &h5file, std::string group_name, std::string attr_name, std::string attr_data)
    {
        RtnStatus rtn_status;
        try
        {
            H5::Group root_group = h5file.openGroup(group_name);
            H5::DataSpace dataspace(H5S_SCALAR);
            H5::StrType type(H5::PredType::C_S1, attr_data.size());
            H5::Attribute attr = root_group.createAttribute(attr_name,type,dataspace); 
            attr.write(type,attr_data);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }
        return rtn_status;
    }


    RtnStatus OutscanData::add_dataset_attribute(H5::H5File &h5file, std::string dataset_name, std::string attr_name, std::string attr_data)
    {
        RtnStatus rtn_status;
        try
        {
            H5::DataSet dataset = h5file.openDataSet(dataset_name);
            H5::DataSpace dataspace(H5S_SCALAR);
            H5::StrType type(H5::PredType::C_S1, attr_data.size());
            H5::Attribute attr = dataset.createAttribute(attr_name,type,dataspace); 
            attr.write(type,attr_data);
        }
        catch (H5::Exception &error)
        {
            std::ostringstream error_oss;
            error_oss << "Error: " << __PRETTY_FUNCTION__ << ", ";
            error_oss << error.getDetailMsg();
            rtn_status.set_error_msg(error_oss.str());
            rtn_status.set_success(false);
        }
        return rtn_status;
    }
    

} // namespace mctl

