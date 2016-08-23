#ifndef MOTION_CONFIG_HPP
#define MOTINO_CONFIG_HPP

#include "motion_constants.hpp"

#include <map>

namespace motion
{
    class MotionConfig
    {
        public:
            MotionConfig();

            bool homing_enabled(Axis axis);
            void set_homing_enabled(Axis axis, bool value);

            Unit axis_unit(Axis axis);
            void set_axis_unit(Axis axis, Unit unit);

            double axis_conversion(Axis axis);
            void set_axis_conversion(Axis axis, double value);


        protected:
            std::map<Axis,bool> homing_enabled_map_;
            std::map<Axis,Unit> axis_to_unit_map_;
            std::map<Axis,double> axis_to_unit_conversion_map_; 
    };

    // Utility functions
    // ----------------------------------------------------------
    std::string operating_mode_to_string(OperatingMode mode);
    std::string axis_to_string(Axis axis);
    std::string unit_to_string(Unit unit);

}

#endif
