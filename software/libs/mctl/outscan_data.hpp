#ifndef OUTSCAN_DATA_HPP
#define OUTSCAN_DATA_HPP
#include "rtn_status.hpp"
#include "configuration.hpp"
#include "command_data.hpp"
#include <cstdint>
#include <string>
#include <deque>
#include <armadillo>

namespace H5 { class H5File; } 

namespace mctl
{

    struct DevToHostMsg;
    struct HostToDevMsg;

    class OutscanData
    {
        public:

            OutscanData();
            arma::Col<double> time();
            arma::Mat<double> stepper_position();
            arma::Mat<double> stepper_velocity();
            arma::Mat<double> pwm_position();
            arma::Mat<double> analog_input();
            arma::Mat<double> force_and_torque();
            arma::Mat<double> stepper_position_cmd();
            arma::Mat<double> stepper_velocity_cmd();
            arma::Mat<uint8_t> dio_cmd();
            arma::Col<uint8_t> status();
            arma::Col<uint8_t> count();
            arma::Col<uint8_t> command();
            arma::Col<uint16_t> command_data(); 

            Configuration config();
            void set_config(Configuration new_config);
            void update(DevToHostMsg msg, CommandData cmd);
            void clear();

            RtnStatus save(std::string filename);

        protected:

            std::deque<double> time_;
            std::deque<std::vector<double>> stepper_position_;
            std::deque<std::vector<double>> stepper_velocity_;
            std::deque<std::vector<double>> pwm_position_;
            std::deque<std::vector<double>> analog_input_;
            std::deque<std::vector<double>> stepper_position_cmd_;
            std::deque<std::vector<double>> stepper_velocity_cmd_;
            std::deque<std::vector<uint8_t>> dio_cmd_;
            std::deque<uint8_t> status_;
            std::deque<uint8_t> count_;
            std::deque<uint8_t> command_;
            std::deque<uint16_t> command_data_;
            Configuration config_;

            const std::string unit_attr_name_ = std::string("unit");
            const std::string axis_attr_name_ = std::string("axis");
            const std::string date_attr_name_ = std::string("date");

            arma::Mat<double> stepper_position_t();
            arma::Mat<double> stepper_velocity_t();
            arma::Mat<double> pwm_position_t();
            arma::Mat<double> analog_input_t();
            arma::Mat<double> force_and_torque_t();
            arma::Mat<double> stepper_position_cmd_t();
            arma::Mat<double> stepper_velocity_cmd_t();
            arma::Mat<uint8_t> dio_cmd_t();

            RtnStatus open_h5file(H5::H5File &h5file, std::string filename);

            RtnStatus add_time_dataset(H5::H5File &h5file);
            RtnStatus add_stepper_position_dataset(H5::H5File &h5file);
            RtnStatus add_stepper_velocity_dataset(H5::H5File &h5file);
            RtnStatus add_pwm_position_dataset(H5::H5File &h5file);
            RtnStatus add_analog_input_dataset(H5::H5File &h5file);
            RtnStatus add_force_and_torque_dataset(H5::H5File &h5file);
            RtnStatus add_stepper_position_cmd_dataset(H5::H5File &h5file);
            RtnStatus add_stepper_velocity_cmd_dataset(H5::H5File &h5file);
            RtnStatus add_dio_cmd_dataset(H5::H5File &h5file);
            RtnStatus add_status_dataset(H5::H5File &h5file);
            RtnStatus add_count_dataset(H5::H5File &h5file);
            RtnStatus add_command_dataset(H5::H5File &h5file);
            RtnStatus add_command_data_dataset(H5::H5File &h5file);

            RtnStatus add_date_attribute(H5::H5File &h5file);
            RtnStatus add_stepper_position_unit_attribute(H5::H5File &h5file, std::string dataset_name);
            RtnStatus add_stepper_velocity_unit_attribute(H5::H5File &h5file, std::string dataset_name);
            RtnStatus add_stepper_axis_attribute(H5::H5File &h5file, std::string dataset_name);
            RtnStatus add_pwm_unit_attribute(H5::H5File &h5file, std::string dataset_name);
            RtnStatus add_pwm_axis_attribute(H5::H5File &h5file, std::string dataset_name);
            RtnStatus add_analog_input_unit_attribute(H5::H5File &h5file, std::string dataset_name);
            RtnStatus add_force_and_torque_attribute(H5::H5File &h5file, std::string dataset_name);

            RtnStatus add_col_uint8_dataset(H5::H5File &h5file, arma::Col<uint8_t> &col, std::string name);
            RtnStatus add_col_uint16_dataset(H5::H5File &h5file, arma::Col<uint16_t> &col, std::string name);
            RtnStatus add_col_double_dataset(H5::H5File &h5file, arma::Col<double> &col, std::string name);

            RtnStatus add_mat_uint8_dataset(H5::H5File &h5file, arma::Mat<uint8_t> &mat_t, std::string name);
            RtnStatus add_mat_double_dataset(H5::H5File &h5file, arma::Mat<double> &mat_t, std::string name);

            RtnStatus add_group_attribute(H5::H5File &h5file, std::string group_name, std::string attr_name, std::string attr_data);
            RtnStatus add_dataset_attribute(H5::H5File &h5file, std::string dataset_name, std::string attr_name, std::string attr_data);

    };


    // Utility functions
    //---------------------------------------------------------------------------------------

    template<typename T>
    arma::Col<T> col_from_deque(std::deque<T> deque)
    {
        arma::Col<T> col(deque.size());
        int index = 0;
        for (auto val : deque)
        {
            col(index) = val;
            index++;
        }
        return col;
    }

    template<typename T>
    arma::Mat<T> mat_from_deque(std::deque<std::vector<T>> deque, int vector_size)
    {
        arma::Mat<T> mat;
        if (deque.size() == 0)
        {
            return mat;
        }
        int n_rows = deque.size();
        int n_cols = deque[0].size();
        if (n_cols != vector_size)
        {
            return mat;
        }

        int index = 0;
        mat.resize(n_rows,n_cols);

        for (auto row_vec : deque)
        {
            if (row_vec.size() == vector_size)
            {
                mat.row(index) = arma::Row<T>(row_vec);
            }
            else
            {
                mat.reset();
                break;
            }
            index ++;
        }
        return mat;
    }
    
    template<typename T>
    arma::Mat<T>  mat_trans_from_deque(std::deque<std::vector<T>> deque, int vector_size)
    {
        // Returns transpose of matrix - used for writing hdf5 data
        arma::Mat<T> mat;
        if (deque.size() == 0)
        {
            return mat;
        }
        int n_cols = deque.size();
        int n_rows = deque[0].size();
        if (n_rows != vector_size)
        {
            return mat;
        }

        int index = 0;
        mat.resize(n_rows,n_cols);

        for (auto col_vec : deque)
        {
            if (col_vec.size() == vector_size)
            {
                mat.col(index) = arma::Col<T>(col_vec);
            }
            else
            {
                mat.reset();
                break;
            }
            index ++;
        }
        return mat;
    }



} // namespace mctl



#endif
