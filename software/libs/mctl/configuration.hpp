#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "constants.hpp"
#include "ft_sensor_cal.hpp"
#include "rtn_status.hpp"
#include "json/json.hpp"

#include <map>
#include <vector>
#include <armadillo>
#include <iosfwd>

using json = nlohmann::json;


namespace mctl
{
    class Configuration
    {
        public:

            Configuration();

            RtnStatus load(); // Loads default in .mctl dir
            RtnStatus load(std::string filename);
            RtnStatus load(json config_json);

            std::string file();
            std::string dir();
            std::string file_contents();

            bool homing_enabled(Axis axis);
            void set_homing_enabled(Axis axis, bool value);

            Unit axis_unit(Axis axis);
            void set_axis_unit(Axis axis, Unit unit);
            std::string axis_unit_string(Axis axis);
            std::string axis_name(Axis axis);
            
            Unit analog_input_unit();
            void set_analog_input_unit(Unit unit);
            std::string analog_input_unit_string();

            double index_to_unit(Axis axis, int32_t index);
            std::vector<double> index_to_unit(std::vector<int32_t> index_vec);
            std::map<Axis,double> index_to_unit(std::map<Axis,int32_t> index_map);
            arma::Row<double> index_to_unit(arma::Row<int32_t> index_vec);
            arma::Mat<double> index_to_unit(arma::Mat<int32_t> index_mat);

            int32_t unit_to_index(Axis axis, double  value);
            std::vector<int32_t> unit_to_index(std::vector<double> value_vec);
            std::map<Axis,int32_t> unit_to_index(std::map<Axis,double> value_map);
            arma::Row<int32_t> unit_to_index(arma::Row<double> value_vec);
            arma::Mat<int32_t> unit_to_index(arma::Mat<double> value_mat);

            double analog_int_to_volt(uint16_t value_int);
            std::vector<double> analog_int_to_volt(std::vector<uint16_t> int_vec);
            arma::Row<double> analog_int_to_volt(arma::Row<uint16_t> int_vec);
            arma::Mat<double> analog_int_to_volt(arma::Mat<uint16_t> int_mat);

            uint16_t analog_volt_to_int(double value_volt);
            std::vector<uint16_t> analog_volt_to_int(std::vector<double> volt_vec);
            arma::Row<uint16_t> analog_volt_to_int(arma::Row<double> volt_vec);
            arma::Mat<uint16_t> analog_volt_to_int(arma::Mat<double> volt_mat);

            // DEVEL - NOT DONE
            // -------------------------------------------------------------------------
            bool have_ft_sensor_cal();
            std::string ft_sensor_cal_info();
            RtnStatus get_force_and_torque_units(std::vector<std::string> &units_vec);
            RtnStatus get_force_and_torque(std::vector<double> ain_vec, std::vector<double> &ft_vec);
            RtnStatus get_force_and_torque(arma::Row<double> ain_vec, arma::Row<double> &ft_vec);
            RtnStatus get_force_and_torque(arma::Mat<double> ain_mat, arma::Mat<double> &ft_vec);
            // -------------------------------------------------------------------------

            double axis_conversion(Axis axis);
            double axis_conversion_inv(Axis axis);
            void set_axis_conversion(Axis axis, double value);

            double homing_backoff(Axis axis);
            void set_homing_backoff(Axis axis, double value);

            int32_t gain();
            void set_gain(int32_t gain);

            int outscan_start_delay();
            void set_outscan_start_delay(int value);

            int outscan_num_bias_samples();
            void set_outscan_num_bias_samples(int value);

            double analog_input_scale();
            void set_analog_input_scale(double value);

            double analog_input_offset();
            void set_analog_input_offset(double value);

            std::map<Axis,double> min_position_map();
            std::map<Axis,double> max_position_map();
            std::map<Axis,double> max_speed_map();
            std::map<Axis,double> max_accel_map();
            std::map<Axis,int> homing_direction_map();

            std::string info_string();

        protected:

            std::string config_dir_;
            std::string config_file_;

            std::map<Axis,Unit> axis_to_unit_map_;
            std::map<Axis,double> axis_to_unit_conversion_map_; 

            std::map<Axis,bool> homing_enabled_map_;
            std::map<Axis,double> homing_backoff_map_;

            std::map<Axis,double> min_position_map_;
            std::map<Axis,double> max_position_map_;
            std::map<Axis,double> max_speed_map_;
            std::map<Axis,double> max_accel_map_;
            std::map<Axis,int> homing_direction_map_;

            FT_SensorCal ft_sensor_cal_;
            
            int32_t gain_;
            int outscan_start_delay_; 
            int outscan_num_bias_samples_;     

            double analog_input_scale_;
            double analog_input_offset_;
            Unit analog_input_unit_;

            std::map<Axis,int> axis_to_joystick_map_;
            std::map<Axis,bool> axis_to_joystick_invert_map_;
            std::string joystick_device_;

            RtnStatus load_homing_enabled(json config_json);
            RtnStatus load_ft_sensor_cal(json config_json);
            RtnStatus load_min_position(json config_json);
            RtnStatus load_max_position(json config_json);
            RtnStatus load_home_direction(json config_json);
            RtnStatus load_max_speed(json config_json);
            RtnStatus load_max_accel(json config_json);
            RtnStatus load_outscan_config(json config_json);
            RtnStatus load_joystick_config(json config_json);
            RtnStatus load_stepper_values(json config_json, std::string key, std::map<Axis,double> &value_map);
            RtnStatus get_config_file_ifstream(std::string filename, std::ifstream &config_ifs);

    };

    // Utility functions
    // ----------------------------------------------------------
    std::string operating_mode_to_string(OperatingMode mode);
    std::string axis_to_string(Axis axis);
    std::string unit_to_string(Unit unit);
    std::string filesep();

}

#endif
