#include "ft_sensor_cal.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace atidaq
{
    extern "C" 
    { 
        #include "ftconfig.h" 
    }
}

namespace mctl
{

    // Untility functions
    std::vector<char> string_to_vector(std::string in_str);

    // Static public memebers
    const std::string FT_SensorCal::DefaultForceUnits("N");
    const std::string FT_SensorCal::DefaultTorqueUnits("N-m"); 
    const FT_ToolTransform FT_SensorCal::DefaultToolTransform = FT_ToolTransform();
    const bool FT_SensorCal::DefaultTemperatureComp = false;
    const int FT_SensorCal::FT_VectorSize = 6;
    const int FT_SensorCal::DisplayFloatPrecision = 5;
    const int FT_SensorCal::DisplayMatrixColumnWidth = FT_SensorCal::DisplayFloatPrecision+7;

    // FT_SensorCal public methods
    // --------------------------------------------------------------------------------------------

    FT_SensorCal::FT_SensorCal()
    { }


    RtnStatus FT_SensorCal::load(std::string filename)
    {
        RtnStatus rtn_status;

        // Create calibration structure and load calibration from file
        std::vector<char> filename_vec = string_to_vector(filename);
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
        if (bias_vec.size() != FT_VectorSize)
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
            units = std::string("");
            return rtn_status;
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
            units = std::string("");
            return rtn_status;
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
        if (!is_initialized(rtn_status))
        {
            return rtn_status;
        }
        else
        {
            atidaq::Transform atidaq_transform = cal_.get() -> cfg.UserTransform;
            std::vector<double> ttvec(FT_VectorSize);
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


    RtnStatus FT_SensorCal::get_filename(std::string &filename)
    {
        RtnStatus rtn_status;
        if (!is_initialized(rtn_status))
        {
            return rtn_status;
        }
        filename = filename_;
        return rtn_status;
    }


    RtnStatus FT_SensorCal::get_info_string(std::string &info)
    {
        RtnStatus rtn_status;
        if (!is_initialized())
        {
            return rtn_status;
        }

        std::stringstream ss;
        ss << std::setprecision(DisplayFloatPrecision);
        ss << std::scientific;

        ss << "serial number:     " << cal_.get() -> Serial << std::endl;
        ss << "body style:        " << cal_.get() -> BodyStyle << std::endl;
        ss << "calibration:       " << cal_.get() -> PartNumber << std::endl;
        ss << "calibration date:  " << cal_.get() -> CalDate << std::endl;
        ss << "family             " << cal_.get() -> Family << std::endl;
        ss << "number channels:   " << cal_.get() -> rt.NumChannels << std::endl;
        ss << "number axes:       " << cal_.get() -> rt.NumAxes << std::endl;
        ss << "force units:       " << cal_.get() -> cfg.ForceUnits << std::endl;
        ss << "torque units:      " << cal_.get() -> cfg.TorqueUnits << std::endl;
        ss << "temperature comp:  " << (cal_.get() -> TempCompAvailable ? "Yes" : "No") << std::endl;;
        ss << "filename:          " << filename_ << std::endl;

        ss << std::endl;
        ss << "calibration matrix" << std::endl;
        for (int i=0; i<cal_.get()->rt.NumAxes; i++)
        {
            std::string axis_name = cal_.get() -> AxisNames[i];
            ss << axis_name << ": ";
            for (int j=0; j<cal_.get()->rt.NumAxes;j++)
            {
                ss << std::setw(DisplayMatrixColumnWidth) << std::setfill(' ');
                ss << cal_.get() -> rt.working_matrix[i][j] << " ";
            }
            ss << std::endl;
        }

        ss << std::endl;
        ss << "rated loads" << std::endl;
        for (int i=0; i<cal_.get()->rt.NumAxes; i++)
        {
            std::string units;
            if (cal_.get() -> AxisNames[i][0] == 'F')
            {
                units = std::string(cal_.get() -> ForceUnits);
            }
            else
            {
                units = std::string(cal_.get() -> TorqueUnits);
            }
            std::string axis_name(cal_.get() -> AxisNames[i]);
            float max_load = cal_.get() -> MaxLoads[i];
            ss << axis_name << ": " << max_load << " " << units << std::endl;
        }


        if (cal_.get() -> TempCompAvailable) 
        {
            ss << std::endl;
            ss << "temperature compensation bias slopes: " << std::endl;;
            for (int i=0; i<(cal_.get()->rt.NumAxes); i++)
            {
                ss << std::setw(DisplayMatrixColumnWidth) << std::setfill(' ');
                ss << (cal_.get()->rt.bias_slopes[i]) << " ";
            }
            ss << std::endl;
            ss << "temperature compensation gain slopes: " << std::endl;;
            for (int i=0; i<(cal_.get()->rt.NumAxes); i++)
            {
                ss << std::setw(DisplayMatrixColumnWidth) << std::setfill(' ');
                ss << (cal_.get()->rt.gain_slopes[i]) << " ";
            }
            ss << std::endl;
        }

        ss << std::endl;
        ss << "bias vector: " << std::endl; 
        for (int i=0; i<(cal_.get()->rt.NumAxes); i++)
        {
            std::string units;
            if (cal_.get() -> AxisNames[i][0] == 'F')
            {
                units = std::string(cal_.get() -> cfg.ForceUnits);
            }
            else
            {
                units = std::string(cal_.get() -> cfg.TorqueUnits);
            }
            std::string axis_name(cal_.get() -> AxisNames[i]);
            float bias = cal_.get() -> rt.bias_vector[i];
            ss << axis_name << ": " << bias << " " << units << std::endl;
        }

        ss << std::endl;
        ss << "tool transform" << std::endl;
        for (int i=0; i<cal_.get()->rt.NumAxes; i++)
        {
            std::string units;
            if (cal_.get() -> AxisNames[i][0] == 'F')
            {
                units = std::string(cal_.get() -> cfg.UserTransform.DistUnits);
            }
            else
            {
                units = std::string(cal_.get() -> cfg.UserTransform.AngleUnits);
            }
            std::string axis_name(cal_.get() -> AxisNames[i]);
            float ttval= cal_.get() -> cfg.UserTransform.TT[i];
            ss << axis_name << ": " << ttval << " " << units << std::endl;
        }
        info = ss.str();
        return rtn_status;
    }

    void FT_SensorCal::print_info_string()
    {
        std::string info;
        RtnStatus rtn_status = get_info_string(info);
        if (rtn_status.success())
        {
            std::cout << std::endl << info << std::endl;
        }
        else
        {
            std::cout << rtn_status.error_msg() << std::endl;
        }
    }

    RtnStatus FT_SensorCal::convert(std::vector<double> ain_vec, std::vector<double> &ft_vec)
    {
        RtnStatus rtn_status;
        if (ain_vec.size()!= FT_VectorSize)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: analog input vector size incorrect");
        }
        else
        {
            std::vector<float> ain_float_vec(ain_vec.begin(), ain_vec.end());
            std::vector<float> ft_float_vec(FT_VectorSize);
            atidaq::ConvertToFT(cal_.get(), ain_float_vec.data(), ft_float_vec.data());
            ft_vec = std::vector<double>(ft_float_vec.begin(), ft_float_vec.end()); 
        }
        return rtn_status;
    }


    RtnStatus FT_SensorCal::convert(arma::Row<double> ain_vec, arma::Row<double> &ft_vec)
    {
        RtnStatus rtn_status;
        if (ain_vec.size() != FT_VectorSize)
        {
            rtn_status.set_success(false);
            rtn_status.set_error_msg("error: analog input vector size incorrect");
        }
        else
        {
            arma::Row<float> ain_float_vec = arma::conv_to<arma::Row<float>>::from(ain_vec);
            arma::Row<float> ft_float_vec(FT_VectorSize,arma::fill::zeros);
            atidaq::ConvertToFT(cal_.get(),ain_float_vec.memptr(),ft_float_vec.memptr());
            ft_vec = arma::conv_to<arma::Row<double>>::from(ft_float_vec);
        }
        return rtn_status;
    }


    RtnStatus FT_SensorCal::convert(arma::Mat<double> ain_mat, arma::Mat<double> &ft_mat)
    {
        RtnStatus rtn_status;
        if (ain_mat.n_cols != FT_VectorSize)
        {
            std::stringstream ss;
            ss << "error: analog input matrix must have n_cols == " << FT_VectorSize;
            rtn_status.set_success(false);
            rtn_status.set_error_msg(ss.str());
        }
        else
        {
            if ((ft_mat.n_cols != ain_mat.n_cols) || (ft_mat.n_rows != ain_mat.n_rows))
            {
                ft_mat.resize(ain_mat.n_rows, ain_mat.n_cols);
            }
            for (int i=0; i<ain_mat.n_rows; i++)
            {
                arma::Row<double> ain_row = ain_mat.row(i);
                arma::Row<double> ft_row;
                rtn_status = convert(ain_row,ft_row);
                if (!rtn_status.success())
                {
                    break;
                }
                ft_mat.row(i) = ft_row;
            }
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
