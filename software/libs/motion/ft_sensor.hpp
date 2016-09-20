#ifndef FT_SENSOR_HPP
#define FT_SENSOR_HPP

#include "rtn_status.hpp"
#include <memory>
#include <vector>
#include <iostream>

extern "C" 
{ 
    #include "ftconfig.h" 
}

struct CalibrationDeleter
{
    void operator()(Calibration *cal) 
    {
        std::cout << "deleting calibration" << std::endl;
        destroyCalibration(cal);

    };
};


namespace motion
{
    class FTSensor
    {
        public:

            FTSensor();
            RtnStatus set_calibration(std::string filename); 
            RtnStatus set_force_units(std::string units);

        protected:

            bool has_cal_ = false;
            std::unique_ptr<Calibration,CalibrationDeleter> cal_;


    };
}



#endif
