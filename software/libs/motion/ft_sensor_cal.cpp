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

        rtn_status = set_tool_transform(DefaultToolTransform);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        rtn_status = set_temperature_comp(DefaultTemperatureComp);
        if (!rtn_status.success())
        {
            return rtn_status;
        }

        std::vector<double> bias_vec = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        rtn_status = set_bias(bias_vec);
        
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
        std::vector<char> rot_units_vec = trans.rotation_units_vec();
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


    RtnStatus FT_SensorCal::set_bias(double fx, double fy, double fz, double tx, double ty, double tz)
    {
        RtnStatus rtn_status;
        std::vector<double> bias_vec = {fx, fy, fz, tx, ty, tz};
        return set_bias(bias_vec);
    }


    RtnStatus FT_SensorCal::set_bias(std::vector<double> bias_vec)
    {
        RtnStatus rtn_status;
        if (!is_initialized(rtn_status))
        {
            return rtn_status;
        }
        if (bias_vec.size() != 6)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: bias vector size incorrect");
            return rtn_status;
        }
        std::vector<float> bias_vec_flt(bias_vec.begin(), bias_vec.end());
        atidaq::Bias(cal_.get(),bias_vec_flt.data());
        return rtn_status;
    }


    RtnStatus FT_SensorCal::set_bias(arma::Row<double> bias_vec)
    {
        RtnStatus rtn_status;
        std::vector<double> bias_vec_std(bias_vec.begin(), bias_vec.end());
        return set_bias(bias_vec_std);
    }


    RtnStatus FT_SensorCal::get_force_units(std::string &units)
    {
        RtnStatus rtn_status;
        if (!is_initialized())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: tool transform in un-initialized");
            units = std::string("");
        }
        else
        {
            units = std::string(cal_.get()->cfg.ForceUnits);
        }
        return rtn_status;
    }


    RtnStatus FT_SensorCal::get_torque_units(std::string &units)
    {
        RtnStatus rtn_status;
        if (!is_initialized())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: tool transform in un-initialized");
            units = std::string("");
        }
        else
        {
            units = std::string(cal_.get()->cfg.TorqueUnits);
        }
        return rtn_status;
    }


    RtnStatus FT_SensorCal::get_tool_transform(FT_ToolTransform &trans)
    {
        RtnStatus rtn_status;
        if (!is_initialized())
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: tool transform in un-initialized");
        }
        else
        {
            atidaq::Transform atidaq_transform = cal_.get() -> cfg.UserTransform;
            std::vector<double> ttvec(6);
            for (int i=0; i<ttvec.size(); i++)
            {
                ttvec[i] = double(atidaq_transform.TT[i]);
            }
            std::string position_units = atidaq_transform.DistUnits;
            std::string rotation_units = atidaq_transform.AngleUnits;
            trans = FT_ToolTransform(ttvec,position_units,rotation_units);
        }
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
