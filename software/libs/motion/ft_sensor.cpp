#include "ft_sensor.hpp"
#include <iostream>

namespace motion
{

    FTSensor::FTSensor()
    { }

    RtnStatus FTSensor::set_calibration(std::string filename)
    {
        RtnStatus rtn_status;

        std::cout << "create calibration" << std::endl;
        std::vector<char> filename_vec(filename.begin(), filename.end());
        cal_ = std::unique_ptr<Calibration,CalibrationDeleter>(createCalibration(filename_vec.data(),1));
        if (cal_ == nullptr)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: unable to create calibration");
            return rtn_status;
        }

        rtn_status = set_force_units("N");
        if (!rtn_status.success())
        {
            return rtn_status;
        }


        return rtn_status;

    }

    RtnStatus FTSensor::set_force_units(std::string units)
    {
        RtnStatus rtn_status;
        std::vector<char> units_vec(units.begin(), units.end());
        switch (SetForceUnits(cal_.get(),units_vec.data())) 
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

}
