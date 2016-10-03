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
        outscan_num_bias_samples_ = DefaultOutscanNumBiasSamples;

        analog_input_scale_ = DefaultAnalogInputScale;
        analog_input_offset_ = DefaultAnalogInputOffset;
        analog_input_unit_ = DefaultAnalogInputUnit; 

        config_dir_ = std::string("");
        config_file_ = std::string("");

        axis_to_joystick_map_ = DefaultAxisToJoystickMap;
        axis_to_joystick_invert_map_ = DefaultAxisToJoystickInvertMap;
        joystick_device_ = DefaultJoystickDevice;

        for (auto stepper : StepperList)
        {
            min_position_map_[stepper] = DefaultStepperMinimumPosition[stepper];
            max_position_map_[stepper] = DefaultStepperMaximumPosition[stepper];
            max_speed_map_[stepper] = DefaultStepperMaximumSpeed[stepper];
            max_accel_map_[stepper] = DefaultStepperMaximumAccel[stepper];
            home_direction_map_[stepper] = DefaultHomingDirection[stepper];
        } 
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

        std::ifstream config_ifs;
        rtn_status = get_config_file_ifstream(filename, config_ifs);
        if (!rtn_status.success())
        {
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

        filesystem::path config_path(filename);
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

        rtn_status = load_homing_enabled(config_json);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_outscan_config(config_json);
        if (!rtn_status.success())
        {
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

        rtn_status = load_home_direction(config_json);
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
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_outscan_config(config_json);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = load_joystick_config(config_json);

        return rtn_status;
    }


    std::string Configuration::file()
    {
        return config_file_;
    }


    std::string Configuration::dir()
    {
        return config_dir_;
    }

    std::string Configuration::file_contents()
    {
        std::stringstream ss;

        std::ifstream config_ifs;
        RtnStatus rtn_status = get_config_file_ifstream(config_file_, config_ifs);
        if (rtn_status.success())
        {
            std::string line;
            while (std::getline(config_ifs,line))
            {
                ss << line << std::endl;
            }
        }
        return ss.str();
    }


    std::string Configuration::info_string()
    {
        std::string pad(" ");
        std::stringstream ss;
        ss << std::setprecision(8) << std::fixed;

        ss << "axis units: " << std::endl;
        for (auto kv : axis_to_unit_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            std::string unit = UnitToStringMap[kv.second];
            ss << pad << name << " = " << unit << std::endl;
        }

        ss << std::endl;
        ss << "axis to unit conversion: " << std::endl;
        for (auto kv : axis_to_unit_conversion_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }

        ss << std::endl;
        ss << "homing enabled: " << std::endl;
        ss << std::boolalpha;
        for (auto kv : homing_enabled_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }
        ss << std::noboolalpha;

        ss << std::endl;
        ss << "homing backoff: " << std::endl;
        for (auto kv : homing_backoff_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }

        ss << std::endl;
        ss << "min position: " << std::endl;
        for (auto kv : min_position_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }

        ss << std::endl;
        ss << "max position: " << std::endl;
        for (auto kv : max_position_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }

        ss << std::endl;
        ss << "home direction: " << std::endl;
        for (auto kv : home_direction_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }

        ss << std::endl;
        ss << "max speed: " << std::endl;
        for (auto kv : max_speed_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }

        ss << std::endl;
        ss << "max accel: " << std::endl;
        for (auto kv : max_accel_map_)
        {
            std::string name = AxisToStringMap[kv.first];
            ss << pad << name << " = " << kv.second << std::endl;
        }

        ss << std::endl;
        ss << "axis to joystick map:" << std::endl;
        for (auto kv : axis_to_joystick_map_)
        {
            if (kv.second >= 0)
            {
                std::string name = AxisToStringMap[kv.first];
                ss << pad << name << " = " << kv.second << std::endl;
            }
        }

        ss << std::endl;
        ss << "axis joystick invert:" << std::endl;
        ss << std::boolalpha;
        for (auto kv : axis_to_joystick_invert_map_)
        {
            if (axis_to_joystick_map_[kv.first] >= 0)
            {
                std::string name = AxisToStringMap[kv.first];
                ss << pad << name << " = " << kv.second << std::endl;
            }
        }
        ss << std::noboolalpha;
        
        ss << std::endl;
        ss << "config file:             " << config_file_ << std::endl;
        std::string ft_cal_file;
        RtnStatus rtn_status = ft_sensor_cal_.get_filename(ft_cal_file);
        ss << "ft_calibration:          ";
        if (rtn_status.success())
        {
            ss << ft_cal_file;
        }
        else
        {
            ss << "none";
        }
        ss << std::endl;
        ss << "joystick device:         " << joystick_device_ << std::endl;
        ss << "outscan start delay:     " << outscan_start_delay_ << " (ms)" << std::endl;
        ss << "outscan # bias samples:  " << outscan_num_bias_samples_ << std::endl;
        return ss.str();
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


    bool Configuration::have_ft_sensor_cal()
    {
        return ft_sensor_cal_.is_initialized();
    }


    std::string Configuration::ft_sensor_cal_info()
    {
        std::string info;
        RtnStatus rtn_status = ft_sensor_cal_.get_info_string(info);
        if (!rtn_status.success())
        {
            info = std::string("ft_senosr_cal: none");
        }
        return info;
    }


    RtnStatus Configuration::get_force_and_torque_units(std::vector<std::string> &units_vec)
    {
        return ft_sensor_cal_.get_units(units_vec);
    }


    RtnStatus Configuration::get_force_and_torque(std::vector<double> ain_vec, std::vector<double> &ft_vec)
    {
        RtnStatus rtn_status;
        if (ain_vec.size() < FT_SensorCal::AinVectorSize)
        {
            std::ostringstream oss;
            oss << "error:  analog input vector size must >= " << FT_SensorCal::AinVectorSize;
            rtn_status.set_success(false);
            rtn_status.set_error_msg(oss.str());
        }
        else
        {
            rtn_status = ft_sensor_cal_.convert(ain_vec, ft_vec);
        }

        if (!rtn_status.success())
        {
            ft_vec = std::vector<double>(FT_SensorCal::FT_VectorSize,0.0); // fill with zeros
        }
        return rtn_status;
    }


    RtnStatus Configuration::get_force_and_torque(arma::Row<double> ain_vec, arma::Row<double> &ft_vec)
    {
        RtnStatus rtn_status;
        if (ain_vec.size() < FT_SensorCal::AinVectorSize)
        {
            std::ostringstream oss;
            oss << "error:  analog input vector size must >= " << FT_SensorCal::AinVectorSize;
            rtn_status.set_success(false);
            rtn_status.set_error_msg(oss.str());
        }
        else
        {
            rtn_status = ft_sensor_cal_.convert(ain_vec,ft_vec);
        }
        if (!rtn_status.success())
        {
            ft_vec = arma::Row<double>(FT_SensorCal::FT_VectorSize,arma::fill::zeros); // fill with zeros
        }
        return rtn_status;
    }


    RtnStatus Configuration::get_force_and_torque(arma::Mat<double> ain_mat, arma::Mat<double> &ft_mat)
    {
        RtnStatus rtn_status;
        if (ain_mat.n_cols < FT_SensorCal::AinVectorSize)
        {
            std::ostringstream oss;
            oss << "error:  analog input matrix n_cols must >= " << FT_SensorCal::AinVectorSize;
            rtn_status.set_success(false);
            rtn_status.set_error_msg(oss.str());
        }
        else
        {
            rtn_status = ft_sensor_cal_.convert(ain_mat,ft_mat);
        }
        if (!rtn_status.success())
        {
            ft_mat = arma::Mat<double>(ain_mat.n_rows,FT_SensorCal::FT_VectorSize,arma::fill::zeros);
        }
        return rtn_status;
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
            if (home_direction_map_[axis] > 0)
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


    int Configuration::outscan_num_bias_samples()
    {
        return outscan_num_bias_samples_;
    }


    void Configuration::set_outscan_num_bias_samples(int value)
    {
        outscan_num_bias_samples_ = value;
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


    RtnStatus Configuration::load_homing_enabled(json config_json)
    {
        RtnStatus rtn_status;
        if (!config_json.is_object())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration root must be object");
            return rtn_status;
        }

        if (!config_json.count("homing_enabled"))
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration does not contain homing_enabled");
            return rtn_status;
        }

        json enabled_json = config_json["homing_enabled"];
        if (!enabled_json.is_object())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration homing_enabled is not object");
            return rtn_status;
        }

        for (auto axis : StepperList)
        {
            std::string axis_name = AxisToStringMap[axis];
            if (!enabled_json.count(axis_name))
            {
                std::ostringstream oss;
                oss << "error: json configuration homing_enabled missing " << axis_name << " axis";
                rtn_status.set_success(false);
                rtn_status.set_error_msg(oss.str());
                return rtn_status;
            }
            if (!enabled_json[axis_name].is_boolean())
            {
                std::ostringstream oss;
                oss << "error: json configuration homing_enabled " << axis_name << " axis is not boolean";
                rtn_status.set_success(false);
                rtn_status.set_error_msg(oss.str());
                return rtn_status;
            }
            homing_enabled_map_[axis] = enabled_json[axis_name];
        }
        return rtn_status;
    }

    RtnStatus Configuration::load_ft_sensor_cal(json config_json)
    {
        RtnStatus rtn_status;
        if (!config_json.is_object())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration root must be object");
            return rtn_status;
        }


        if (!config_json.count("ft_calibration"))
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: configuration does not contain ft_calibration");
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


    RtnStatus Configuration::load_home_direction(json config_json)
    {
        std::map<Axis,double> map_tmp;
        RtnStatus rtn_status = load_stepper_values(config_json,"home_direction", map_tmp);
        if (rtn_status.success())
        {
            for (auto kv : map_tmp)
            {
                if (kv.second == 0)
                {
                    rtn_status.set_success(false);
                    rtn_status.set_error_msg("error: home direction must be nonzero");
                    return rtn_status;
                }
            }

            home_direction_map_.clear();
            for (auto kv : map_tmp)
            {
                if (kv.second > 0)
                {
                    home_direction_map_.emplace(kv.first,1);
                }
                else
                {
                    home_direction_map_.emplace(kv.first,-1);
                }
            }
        }
        return rtn_status;
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

    RtnStatus Configuration::load_outscan_config(json config_json)
    {
        RtnStatus rtn_status;
        if (!config_json.is_object())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration root must be object");
            return rtn_status;
        }
        if (config_json.count("outscan"))
        {
            if (!config_json.is_object())
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: json configuration outscan must be object");
                return rtn_status;
            }

            json outscan_json = config_json["outscan"];

            // Get start delay
            if (!outscan_json.count("start_delay_ms"))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: json configuration outscan must contian start_delay");
                return rtn_status;
            }
            if (!outscan_json["start_delay_ms"].is_number())
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: json configuration outscan start_delay_ms must be number");
                return rtn_status;
            }
            int start_delay_tmp = int(outscan_json["start_delay_ms"]);
            if (start_delay_tmp < 0)
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: json configuration outscan start_delay_ms must be >=0");
                return rtn_status;
            }
            outscan_start_delay_ = start_delay_tmp;

            // get number of bias samples
            if(!outscan_json.count("num_bias_samples"))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: json configuration outscan must contain num_bias_samples");
                return rtn_status;
            }
            if (!outscan_json["num_bias_samples"].is_number())
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: json configuration outscan num_bias_samples must be number");
                return rtn_status;
            }
            int num_bias_samples_tmp = int(outscan_json["num_bias_samples"]);
            if (num_bias_samples_tmp < 0)
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: json configuration outscan bias_samples must be >=0");
                return rtn_status;
            }
            outscan_num_bias_samples_ = num_bias_samples_tmp;
        }
        return rtn_status;
    }


    RtnStatus Configuration::load_joystick_config(json config_json)
    {
        RtnStatus rtn_status;
        if (!config_json.is_object())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: json configuration root must be object");
            return rtn_status;
        }
        if (config_json.count("joystick"))
        {
            json joy_json = config_json["joystick"];

            // Get device string
            if (!joy_json.count("device"))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: joystick device not specified");
                return rtn_status;
            }
            if (!joy_json["device"].is_string())
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: joystick device is not string");
                return rtn_status;
            }
            joystick_device_ = joy_json["device"];

            // Get axis to joystick mapping
            if (!joy_json.count("mapping"))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: axis to joystick mapping is missing ");
                return rtn_status;
            }

            json axis_map_json = joy_json["mapping"];
            if (!axis_map_json.is_object())
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: joystick mapping is not object");
                return rtn_status;
            }

            for (auto axis : AxisList)
            {
                std::string axis_name = AxisToStringMap[axis];
                if (axis_map_json.count(axis_name))
                {
                    if (!axis_map_json[axis_name].is_number())
                    {
                        rtn_status.set_success(false);
                        rtn_status.set_error_msg("error: axis to joystick map value must integer");
                        return rtn_status;
                    }
                    else
                    {
                        int value = int(axis_map_json[axis_name]);
                        if (value < 0)
                        {
                            axis_to_joystick_map_[axis] = -1;
                        }
                        else
                        {
                            axis_to_joystick_map_[axis] = value;
                        }
                    }
                }
                else
                {
                    axis_to_joystick_map_[axis] = -1;
                }
            }


            // Get joystick invert mapping
            if (!joy_json.count("invert"))
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: axis to joystick invert assignment is missing ");
                return rtn_status;
            }
            if (!joy_json["invert"].is_object())
            {
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: joystick invert is not object");
                return rtn_status;
            }

            json invert_json = joy_json["invert"];
            for (auto axis : AxisList)
            {
                std::string axis_name = AxisToStringMap[axis];
                if (invert_json.count(axis_name))
                {
                    if (!invert_json[axis_name].is_boolean())
                    {
                        rtn_status.set_success(false);
                        rtn_status.set_error_msg("error: axis to joystick invert value must be boolean");
                        return rtn_status;
                    }
                    axis_to_joystick_invert_map_[axis] = bool(invert_json[axis_name]);
                }
                else
                {
                    axis_to_joystick_invert_map_[axis] = false;
                }
            }

        }
        return rtn_status;
    }


    RtnStatus Configuration::get_config_file_ifstream(std::string filename, std::ifstream &config_ifs)
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
        config_ifs.open(config_path.str(), std::ifstream::in);
        if (!config_ifs.is_open())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: unable to open configuration file");
            return rtn_status;
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
