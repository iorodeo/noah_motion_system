#include "ft_sensor_cal.hpp"
#include <iostream>

namespace motion
{
    // Untility functions
    std::vector<char> string_to_vector(std::string in_str);

    // Static public memebers
    const std::string FT_SensorCal::DefaultForceUnits("N");
    const std::string FT_SensorCal::DefaultTorqueUnits("N-m"); 
    const FT_ToolTransform FT_SensorCal::DefaultToolTransform = FT_ToolTransform();
    const bool FT_SensorCal::DefaultTemperatureComp = false;

    // FT_SensorCal public methods
    // --------------------------------------------------------------------------------------------

    FT_SensorCal::FT_SensorCal()
    { }


    RtnStatus FT_SensorCal::set_from_file(std::string filename)
    {
        RtnStatus rtn_status;

        // Create calibration structure and load calibration from file
        std::vector<char> filename_vec(filename.begin(), filename.end());
        cal_ = std::shared_ptr<atidaq::Calibration>(atidaq::createCalibration(filename_vec.data(),1), atidaq::destroyCalibration);
        if (cal_ == nullptr)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: unable to create calibration");
            return rtn_status;
        }
        std::cout << "1 rtn_status.success() = " << (rtn_status.success()) << std::endl;

        rtn_status = set_force_units(DefaultForceUnits);
        std::cout << "2 rtn_status.success() = " << (rtn_status.success()) << std::endl;
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = set_torque_units(DefaultTorqueUnits);
        std::cout << "3 rtn_status.success() = " << (rtn_status.success()) << std::endl;
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = set_tool_transform(DefaultToolTransform);
        std::cout << "4 rtn_status.success() = " << (rtn_status.success()) << std::endl;
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = set_temperature_comp(DefaultTemperatureComp);
        std::cout << "5 rtn_status.success() = " << (rtn_status.success()) << std::endl;
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        
        filename_ = filename;
        return rtn_status;

    }


    RtnStatus FT_SensorCal::set_force_units(std::string units)
    {
        RtnStatus rtn_status;
        if (!is_initialized(rtn_status))
        {
            return rtn_status;
        }
        std::vector<char> units_vec = string_to_vector(units);
        rtn_status = check_atidaq_rtn(atidaq::SetForceUnits(cal_.get(),units_vec.data())); 
        return rtn_status;
    }


    RtnStatus FT_SensorCal::set_torque_units(std::string units)
    {
        RtnStatus rtn_status;
        if (!is_initialized(rtn_status))
        {
            return rtn_status;
        }
        std::vector<char> units_vec = string_to_vector(units);
        rtn_status = check_atidaq_rtn(atidaq::SetTorqueUnits(cal_.get(),units_vec.data())); 
        return rtn_status;
    } 
    

    RtnStatus FT_SensorCal::set_tool_transform(FT_ToolTransform trans)
    {
        RtnStatus rtn_status;
        if (!is_initialized(rtn_status))
        {
            return rtn_status;
        }
        std::vector<float> trans_vec = trans.as_vector<float>();
        std::vector<char> pos_units_vec = trans.position_units_vec();
        std::vector<char> rot_units_vec = trans.position_units_vec();
        rtn_status = check_atidaq_rtn(atidaq::SetToolTransform(cal_.get(),trans_vec.data(),pos_units_vec.data(),rot_units_vec.data())); 
        return rtn_status;
    }

    RtnStatus FT_SensorCal::set_temperature_comp(bool value)
    {
        RtnStatus rtn_status;
        if (!is_initialized(rtn_status))
        {
            return rtn_status;
        }
        rtn_status = check_atidaq_rtn(atidaq::SetTempComp(cal_.get(),value));
        return rtn_status;
    }



    // FT_SensorCal protected methods
    // --------------------------------------------------------------------------------------------
    
    bool FT_SensorCal::is_initialized()
    {
        return (cal_ != nullptr);
    }


    bool FT_SensorCal::is_initialized(RtnStatus &rtn_status)
    {
        bool rval = true;
        if (cal_ == nullptr)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: calibration is not initialized");
        }
        return rval;
    }

    RtnStatus FT_SensorCal::check_atidaq_rtn(short rtn_code)
    {
        RtnStatus rtn_status;
        switch (rtn_code)
        {
            case 0: 
                // successful completion
                break;  

            case 1: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: invalid calibration struct"); 
                break;

            case 2: 
            case 3:
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: invalid value"); 
                break;
            default: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: unknown/specified issue setting atidaq value"); 
                break;
        }

        return rtn_status;
    }

    // Utility functions
    // --------------------------------------------------------------------------------------------
    std::vector<char> string_to_vector(std::string in_str)
    {
        std::vector<char> out_vec(in_str.length()+1,'\0');
        std::copy(in_str.begin(),in_str.end(),out_vec.begin());
        return out_vec;
    }

}
