#ifndef MOTION_CONFIG_HPP
#define MOTINO_CONFIG_HPP

#include "motion_constants.hpp"

#include <map>
#include <vector>

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

            double index_to_unit(Axis axis, int32_t index);
            std::vector<double> index_to_unit(std::vector<int32_t> index_vec);
            std::map<Axis,double> index_to_unit(std::map<Axis,int32_t> index_map);

            int32_t unit_to_index(Axis axis, double  value);
            std::vector<int32_t> unit_to_index(std::vector<double> value_vec);
            std::map<Axis,int32_t> unit_to_index(std::map<Axis,double> value_map);

            double axis_conversion(Axis axis);
            void set_axis_conversion(Axis axis, double value);

            double homing_backoff(Axis axis);
            void set_homing_backoff(Axis axis, double value);

        protected:
            std::map<Axis,bool> homing_enabled_map_;
            std::map<Axis,Unit> axis_to_unit_map_;
            std::map<Axis,double> axis_to_unit_conversion_map_; 
            std::map<Axis,double> homing_backoff_map_;
    };

    // Utility functions
    // ----------------------------------------------------------
    std::string operating_mode_to_string(OperatingMode mode);
    std::string axis_to_string(Axis axis);
    std::string unit_to_string(Unit unit);

}

#endif
