#ifndef FT_SENSOR_CAL_HPP
#define FT_SENSOR_CAL_HPP

#include "rtn_status.hpp"
#include "ft_tool_transform.hpp"
#include <memory>
#include <vector>
#include <iostream>

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

            FT_SensorCal();
            RtnStatus set_from_file(std::string filename); 
            RtnStatus set_force_units(std::string units);
            RtnStatus set_torque_units(std::string units);
            //RtnStatus set_tool_transform(FT_ToolTransform trans);

        protected:

            bool initialized_ = false;
            std::string filename_ = std::string("");
            std::shared_ptr<atidaq::Calibration> cal_;

    };
}



#endif
