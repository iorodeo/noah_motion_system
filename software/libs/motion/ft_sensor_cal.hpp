#ifndef FT_SENSOR_CAL_HPP
#define FT_SENSOR_CAL_HPP

#include "rtn_status.hpp"
#include "ft_tool_transform.hpp"
#include <memory>
#include <vector>
#include <iostream>
#include <armadillo>

namespace atidaq
{
    extern "C" 
    { 
        #include "ftconfig.h" 
    }
}


namespace motion
{

    class FT_SensorCal
    {
        public:

            static const std::string DefaultForceUnits;
            static const std::string DefaultTorqueUnits; 
            static const FT_ToolTransform DefaultToolTransform;
            static const bool DefaultTemperatureComp;

            FT_SensorCal();
            RtnStatus set_from_file(std::string filename); 
            RtnStatus set_force_units(std::string units);
            RtnStatus set_torque_units(std::string units);
            RtnStatus set_tool_transform(FT_ToolTransform trans);
            RtnStatus set_temperature_comp(bool value);

            RtnStatus set_bias(double fx, double fy, double fz, double tx, double ty, double tz);
            RtnStatus set_bias(std::vector<double> bias_vec);
            RtnStatus set_bias(arma::Row<double> bias_vec);

            RtnStatus get_force_units(std::string &units);
            RtnStatus get_torque_units(std::string &units);
            RtnStatus get_tool_transform(FT_ToolTransform &tran);

        protected:

            std::string filename_ = std::string("");
            std::shared_ptr<atidaq::Calibration> cal_ = nullptr;

            bool is_initialized();
            bool is_initialized(RtnStatus &rtn_status);
            RtnStatus check_atidaq_rtn(short rtn_code);

    };

}

#endif
