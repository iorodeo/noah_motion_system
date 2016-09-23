#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "constants.hpp"
#include "ft_sensor_cal.hpp"
#include "rtn_status.hpp"
#include "json/json.hpp"

#include <map>
#include <vector>
#include <armadillo>

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
            std::vector<double> get_force_and_torque(std::vector<double> analog_input);
            arma::Row<double> get_force_and_torque(arma::Row<double> analog_input);
            arma::Mat<double> get_force_and_torque(arma::Mat<double> analog_input);
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

            double analog_input_scale();
            void set_analog_input_scale(double value);

            double analog_input_offset();
            void set_analog_input_offset(double value);

        protected:

            std::string config_dir_;
            std::string config_file_;

            std::map<Axis,bool> homing_enabled_map_;
            std::map<Axis,Unit> axis_to_unit_map_;
            std::map<Axis,double> axis_to_unit_conversion_map_; 
            std::map<Axis,double> homing_backoff_map_;

            FT_SensorCal ft_sensor_cal_;
            
            int32_t gain_;
            int outscan_start_delay_;

            double analog_input_scale_;
            double analog_input_offset_;
            Unit analog_input_unit_;

            RtnStatus load_ft_sensor_cal(json config_json);

    };

    // Utility functions
    // ----------------------------------------------------------
    std::string operating_mode_to_string(OperatingMode mode);
    std::string axis_to_string(Axis axis);
    std::string unit_to_string(Unit unit);
    std::string filesep();

}

#endif
