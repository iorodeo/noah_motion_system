#include "configuration.hpp"
#include "filesystem/path.h"
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>

namespace mctl
{
    // Configuration public methods
    // ----------------------------------------------------------------------------------
    Configuration::Configuration()
    {
        homing_enabled_map_ = DefaultHomingEnabledMap;
        homing_backoff_map_ = DefaultHomingBackoffMap; 
        axis_to_unit_map_ = DefaultAxisToUnitMap;
        axis_to_unit_conversion_map_ = DefaultAxisToUnitConversionMap;

        gain_ = DefaultGain;
        outscan_start_delay_ = DefaultOutscanStartDelay_ms;

        analog_input_scale_ = DefaultAnalogInputScale;
        analog_input_offset_ = DefaultAnalogInputOffset;
        analog_input_unit_ = DefaultAnalogInputUnit; 

        std::string config_dir_ = std::string("");
        std::string config_file_ = std::string("");

    }


    RtnStatus Configuration::load()
    {
        std::ostringstream path_oss;
        path_oss << std::getenv("HOME") << filesep() ;
        path_oss << DefaultConfigurationDir << filesep();
        path_oss << DefaultConfigurationFile;
        return load(path_oss.str());
    }


    RtnStatus Configuration::load(std::string filename)
    {
        RtnStatus rtn_status;

        filesystem::path config_path(filename);

        // Check that configuration directory and file exist
        if (!config_path.parent_path().exists())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: configuration directory does not exist");
            return rtn_status;
        }

        if (!config_path.exists())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: configuration file does not exist");
            return rtn_status;

        }

        // Open configuration file and read json
        std::ifstream config_ifs(config_path.str(), std::ifstream::in);
        if (!config_ifs.is_open())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: unable to open configuration file");
            return rtn_status;
        }

        json config_json;
        try
        {
            config_ifs >> config_json;
        }
        catch (std::invalid_argument &err)
        {
            std::ostringstream oss;
            oss << "error: unable to load configuration, " << err.what();
            rtn_status.set_success(false);
            rtn_status.set_error_msg(oss.str());
            return rtn_status;
        }

        config_dir_  = config_path.parent_path().str();
        config_file_ = config_path.str();

        rtn_status = load(config_json);
        if (!rtn_status.success())
        {
            config_dir_ = std::string("");
            config_file_ = std::string("");
        }
        return rtn_status;
    }


    RtnStatus Configuration::load(json config_json)
    {
        RtnStatus rtn_status;

        if (!config_json.is_object())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration root must be object");
            return rtn_status;
        }

        rtn_status = load_ft_sensor_cal(config_json);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_min_position(config_json);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_max_position(config_json);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_home_position(config_json);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_max_speed(config_json);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_max_accel(config_json);

        return rtn_status;
    }


    // Configuration protected methods
    // ----------------------------------------------------------------------------------
    bool Configuration::homing_enabled(Axis axis)
    {
        bool rval = false;
        if (homing_enabled_map_.count(axis) > 0)
        {
            rval = homing_enabled_map_[axis];
        }
        return rval;
    }


    void Configuration::set_homing_enabled(Axis axis, bool value)
    {
        if (homing_enabled_map_.count(axis) > 0)
        {
            homing_enabled_map_[axis] = value;
        }
    }


    Unit Configuration::axis_unit(Axis axis)
    {
        Unit unit = NoUnit;
        if (axis_to_unit_map_.count(axis) > 0)
        {
            unit = axis_to_unit_map_[axis];
        }
        return unit;
    }

    void Configuration::set_axis_unit(Axis axis, Unit unit)
    {
        if (axis_to_unit_map_.count(axis) > 0)
        {
            axis_to_unit_map_[axis] = unit;
        }
    }


    std::string Configuration::axis_unit_string(Axis axis)
    {
        Unit unit = axis_unit(axis);
        return unit_to_string(unit);
    }


    std::string Configuration::axis_name(Axis axis)
    {
        return axis_to_string(axis);
    }


    double Configuration::axis_conversion(Axis axis)
    {
        double value = 1.0;
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            value = axis_to_unit_conversion_map_[axis];
        }
        return value;
    }


    Unit Configuration::analog_input_unit()
    {
        return analog_input_unit_;
    }


    void Configuration::set_analog_input_unit(Unit unit)
    {
        analog_input_unit_ = unit;
    }


    std::string Configuration::analog_input_unit_string()
    {
        return unit_to_string(analog_input_unit_);
    }


    double Configuration::axis_conversion_inv(Axis axis)
    {
        double scale_fact = axis_conversion(axis);
        double scale_fact_inv = 1.0;
        if (scale_fact > 0.0)
        {
            scale_fact_inv = 1.0/scale_fact;
        }
        return scale_fact_inv;
    }


    void Configuration::set_axis_conversion(Axis axis, double value)
    {
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            axis_to_unit_conversion_map_[axis] = value;
        }
    }


    double Configuration::index_to_unit(Axis axis, int32_t index)
    {
        return axis_conversion(axis)*index;
    }


    std::vector<double> Configuration::index_to_unit(std::vector<int32_t> index_vec)
    {
        std::vector<double> value_vec(index_vec.size());
        for (int i=0; i<index_vec.size(); i++)
        {
            value_vec[i] = index_to_unit(Axis(i),index_vec[i]);
        }
        return value_vec;
    }


    std::map<Axis,double> Configuration::index_to_unit(std::map<Axis,int32_t> index_map)
    {
        std::map<Axis,double> value_map;
        for (auto kv : index_map)
        {
            double value = index_to_unit(kv.first,kv.second);
            std::pair<Axis,double> kv_new(kv.first,value);
            value_map.insert(kv_new);
        }
        return value_map;
    }


    arma::Row<double> Configuration::index_to_unit(arma::Row<int32_t> index_vec)
    {
        arma::Row<double> value_vec(index_vec.size());
        for (int i=0; i<index_vec.size(); i++)
        {
            value_vec(i) = index_to_unit(Axis(i),index_vec(i));
        }
        return value_vec;
    }


    arma::Mat<double> Configuration::index_to_unit(arma::Mat<int32_t> index_mat)
    {
        arma::Mat<double> value_mat = arma::conv_to<arma::Mat<double>>::from(index_mat);
        for (int i=0; i<value_mat.n_cols; i++)
        {
            value_mat.col(i) = axis_conversion(Axis(i))*value_mat.col(i);
        }
        return value_mat;
    }


    int32_t Configuration::unit_to_index(Axis axis, double  value)
    {
        return int32_t(axis_conversion_inv(axis)*value);
    }


    std::vector<int32_t> Configuration::unit_to_index(std::vector<double> value_vec)
    {
        std::vector<int32_t> index_vec(value_vec.size());
        for (int i=0; i<value_vec.size(); i++)
        {
            index_vec[i] = unit_to_index(Axis(i),value_vec[i]);
        }
        return index_vec;
    }


    std::map<Axis,int32_t> Configuration::unit_to_index(std::map<Axis,double> value_map)
    {
        std::map<Axis,int32_t> index_map;
        for (auto kv : value_map)
        {
            int32_t index = unit_to_index(kv.first,kv.second);
            std::pair<Axis,int32_t> kv_new(kv.first,index);
            index_map.insert(kv_new);
        }
        return index_map;
    }


    arma::Row<int32_t> Configuration::unit_to_index(arma::Row<double> index_vec)
    {
        arma::Row<int32_t> value_vec(index_vec.size());
        for (int i=0; i<index_vec.size(); i++)
        {
            value_vec(i) = unit_to_index(Axis(i),index_vec(i));
        }
        return value_vec;
    }


    arma::Mat<int32_t> Configuration::unit_to_index(arma::Mat<double> value_mat)
    {
        arma::Mat<int32_t> index_mat(value_mat.n_rows,value_mat.n_cols);
        for (int i=0; i<index_mat.n_cols; i++)
        {
            arma::Col<double> index_col_dbl = axis_conversion_inv(Axis(i))*value_mat.col(i);
            index_mat.col(i) = arma::conv_to<arma::Mat<int32_t>>::from(index_col_dbl);
        }
        return index_mat;
    }

    double Configuration::analog_int_to_volt(uint16_t value_int)
    {
        return analog_input_scale_*value_int + analog_input_offset_;
    }


    std::vector<double> Configuration::analog_int_to_volt(std::vector<uint16_t> int_vec)
    {
        std::vector<double> volt_vec(int_vec.size());
        for (int i=0; i<volt_vec.size(); i++)
        {
            volt_vec[i] = analog_int_to_volt(int_vec[i]);
        }
        return volt_vec;
    }


    arma::Row<double> Configuration::analog_int_to_volt(arma::Row<uint16_t> int_vec)
    {
        arma::Row<double> volt_vec(int_vec.size());
        for (int i=0; i<volt_vec.size(); i++)
        {
            volt_vec(i) = analog_int_to_volt(int_vec(i));
        }
        return volt_vec;
    }


    arma::Mat<double> Configuration::analog_int_to_volt(arma::Mat<uint16_t> int_mat)
    {
        arma::Mat<double> volt_mat(int_mat.n_rows, int_mat.n_cols);
        for (int i=0;i<volt_mat.n_rows; i++)
        {
            for (int j=0; j<volt_mat.n_cols; i++)
            {
                volt_mat(i,j) = analog_int_to_volt(int_mat(i,j)); 
            }
        }
        return volt_mat;
    }


    uint16_t Configuration::analog_volt_to_int(double value_volt)
    {
        return uint16_t((value_volt - analog_input_offset_)/analog_input_scale_);
    }


    std::vector<uint16_t> Configuration::analog_volt_to_int(std::vector<double> volt_vec)
    {
        std::vector<uint16_t> int_vec(volt_vec.size());
        for (int i=0; i<int_vec.size(); i++)
        {
            int_vec[i] = analog_volt_to_int(volt_vec[i]);
        }
        return int_vec;
    }


    arma::Row<uint16_t> Configuration::analog_volt_to_int(arma::Row<double> volt_vec)
    {
        arma::Row<uint16_t> int_vec(volt_vec.size());
        for (int i=0; i<int_vec.size(); i++)
        {
            int_vec(i) = analog_volt_to_int(volt_vec(i));
        }
        return int_vec;
    }


    arma::Mat<uint16_t> Configuration::analog_volt_to_int(arma::Mat<double> volt_mat)
    {
        arma::Mat<uint16_t> int_mat(volt_mat.n_rows,volt_mat.n_cols);
        for (int i=0; i<int_mat.n_rows; i++)
        {
            for (int j=0; j<int_mat.n_cols; j++)
            {
                int_mat(i,j) = analog_volt_to_int(volt_mat(i,j));
            }
        }
        return int_mat;
    }


    // DEVEL - NOT DONE
    // -------------------------------------------------------------------------
    
    std::vector<double> Configuration::get_force_and_torque(std::vector<double> analog_input)
    {
        // PLACE HOLDER - NOT FINISHED
        std::vector<double> tmp_vec(ForceAndTorqueRowDim,0.0);
        return tmp_vec;
    }


    arma::Row<double> Configuration::get_force_and_torque(arma::Row<double> analog_input)
    {
        // PLACE HOLDER - NOT FINISHED
        arma::Row<double> tmp_vec(ForceAndTorqueRowDim,arma::fill::zeros);
        return tmp_vec;
    }


    arma::Mat<double> Configuration::get_force_and_torque(arma::Mat<double> analog_input)
    {
        // PLACE HOLDER - NOT FINISHED
        arma::Mat<double> tmp_mat(analog_input.n_rows,ForceAndTorqueRowDim,arma::fill::zeros);
        return tmp_mat;
    }

    // -------------------------------------------------------------------------

    double Configuration::homing_backoff(Axis axis)
    {
        double backoff = 0.0;
        double direction = 1.0;

        if (homing_backoff_map_.count(axis) > 0)
        {
            backoff = homing_backoff_map_[axis];
        }

        if (axis < NumStepper)
        {
            if (HomingDirection[axis] > 0)
            {
                direction = -1.0;
            }
        }
        return direction*backoff;
    }


    void Configuration::set_homing_backoff(Axis axis, double value)
    {
        if (homing_backoff_map_.count(axis) > 0)
        {
            homing_backoff_map_[axis] = fabs(value);
        }
    }


    int32_t Configuration::gain()
    {
        return gain_;
    }


    void Configuration::set_gain(int32_t gain)
    {
        gain_ = gain;
    }


    int Configuration::outscan_start_delay()
    {
        return outscan_start_delay_;
    }


    void Configuration::set_outscan_start_delay(int value)
    {
        outscan_start_delay_ = value;
    }


    double Configuration::analog_input_scale()
    {
        return analog_input_scale_;
    }


    void Configuration::set_analog_input_scale(double value)
    {
        analog_input_scale_ = value;
    }


    double Configuration::analog_input_offset()
    {
        return analog_input_offset_;
    }


    void Configuration::set_analog_input_offset(double value)
    {
        analog_input_offset_ = value;
    }


    RtnStatus Configuration::load_ft_sensor_cal(json config_json)
    {
        RtnStatus rtn_status;

        if (!config_json.count("ft_calibration"))
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: configuration  does not contain ft_calibration");
            return rtn_status;
        }
        if (!config_json["ft_calibration"].is_string())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: ft_calibration must be string");
            return rtn_status;
        }

        std::string ft_calibration_file = config_json["ft_calibration"];
        std::ostringstream cal_file_oss;
        cal_file_oss << config_dir_ << filesep() << ft_calibration_file;
        rtn_status = ft_sensor_cal_.load(cal_file_oss.str());
        return rtn_status;
    }

    RtnStatus Configuration::load_min_position(json config_json)
    {
        return load_stepper_values(config_json,"min_position", min_position_map_);
    }


    RtnStatus Configuration::load_max_position(json config_json)
    {
        return load_stepper_values(config_json,"max_position",max_position_map_);
    }


    RtnStatus Configuration::load_home_position(json config_json)
    {
        return load_stepper_values(config_json,"home_position", home_position_map_);
    }


    RtnStatus Configuration::load_max_speed(json config_json)
    {
        return load_stepper_values(config_json,"max_speed", max_speed_map_);
    }


    RtnStatus Configuration::load_max_accel(json config_json)
    {
        return load_stepper_values(config_json,"max_accel", max_accel_map_);
    }


    RtnStatus Configuration::load_stepper_values(json config_json, std::string key, std::map<Axis,double> &value_map)
    {
        RtnStatus rtn_status;
        if (!config_json.is_object())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration root must be object");
            return rtn_status;
        }

        if (config_json.count(key))
        {
            value_map.clear();
            json key_json = config_json[key];
            for (auto stepper : StepperList)
            {
                std::string stepper_name = AxisToStringMap[stepper];
                if (key_json.count(stepper_name))
                {
                    if (key_json[stepper_name].is_number())
                    {
                        double value = key_json[stepper_name];
                        value_map.emplace(stepper, value);
                    }
                    else
                    {
                        std::ostringstream oss;
                        oss << "error: " << stepper_name << " value for " << key << " is not a number";
                        rtn_status.set_success(false);
                        rtn_status.set_error_msg(oss.str());
                        break;
                    }
                }
            }
        }
        return rtn_status;
    }


    // Utility functions
    // ----------------------------------------------------------------------------------
    std::string operating_mode_to_string(OperatingMode mode)
    {
        if (OperatingModeToStringMap.count(mode) > 0)
        {
            return OperatingModeToStringMap[mode];
        }
        else
        {
            return std::string("mode not found");
        }
    }


    std::string axis_to_string(Axis axis)
    {
        if (AxisToStringMap.count(axis) > 0)
        {
            return AxisToStringMap[axis];
        }
        else
        {
            return std::string("axis not found");
        }
    }


    std::string unit_to_string(Unit unit)
    {
        if (UnitToStringMap.count(unit) > 0)
        {
            return UnitToStringMap[unit];
        }
        else
        {
            return std::string("unit not found");
        }
    }

    std::string filesep()
    {
#ifdef _WIN32
        return std::string("\\");
#else
        return std::string("/");
#endif
    }

}
