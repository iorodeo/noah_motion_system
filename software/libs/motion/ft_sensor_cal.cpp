#include "ft_sensor_cal.hpp"
#include <iostream>

namespace motion
{

    const std::string FT_SensorCal::DefaultForceUnits("N");
    const std::string FT_SensorCal::DefaultTorqueUnits("N-m"); 

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

        // Set units
        rtn_status = set_force_units(DefaultForceUnits);
        if (!rtn_status.success())
        {
            return rtn_status;
        }
        rtn_status = set_torque_units(DefaultTorqueUnits);
        if (!rtn_status.success())
        {
            return rtn_status;
        }


        filename_ = filename;
        initialized_ = true;
        return rtn_status;

    }


    RtnStatus FT_SensorCal::set_force_units(std::string units)
    {
        RtnStatus rtn_status;
        std::vector<char> units_vec(units.begin(), units.end());
        switch (atidaq::SetForceUnits(cal_.get(),units_vec.data())) 
        {
            case 0: 
                // successful completion
                break;  

            case 1: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: invalid calibration struct"); 
                break;

            case 2: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: invalid force units"); 
                break;

            default: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("error: setting force units"); 
                break;
        }
        return rtn_status;
    }


    RtnStatus FT_SensorCal::set_torque_units(std::string units)
    {
        RtnStatus rtn_status;
        std::vector<char> units_vec(units.begin(), units.end());
        switch ( atidaq::SetTorqueUnits(cal_.get(),units_vec.data()) ) 
        {
            case 0: 
                // successful completion
                break;  
            case 1: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("Invalid Calibration struct"); 
                break;
            case 2: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("Invalid torque units"); 
                break;
            default: 
                rtn_status.set_success(false);
                rtn_status.set_error_msg("Unknown error"); 
                break;
        }
        return rtn_status;
    }

}
