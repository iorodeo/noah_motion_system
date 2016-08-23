#include "motion_config.hpp"

namespace motion
{
    // MotionConfig public methods
    // ----------------------------------------------------------------------------------
    MotionConfig::MotionConfig()
    {
        homing_enabled_map_ = DefaultHomingEnabledMap;
        axis_to_unit_map_ = DefaultAxisToUnitMap;
        axis_to_unit_conversion_map_ = DefaultAxisToUnitConversionMap;

    }


    // MotionConfig protected methods
    // ----------------------------------------------------------------------------------
    bool MotionConfig::homing_enabled(Axis axis)
    {
        bool rval = false;
        if (homing_enabled_map_.count(axis) > 0)
        {
            rval = homing_enabled_map_[axis];
        }
        return rval;
    }


    void MotionConfig::set_homing_enabled(Axis axis, bool value)
    {
        if (homing_enabled_map_.count(axis) > 0)
        {
            homing_enabled_map_[axis] = value;
        }
    }


    Unit MotionConfig::axis_unit(Axis axis)
    {
        Unit unit = NoUnit;
        if (axis_to_unit_map_.count(axis) > 0)
        {
            unit = axis_to_unit_map_[axis];
        }
        return unit;
    }

    void MotionConfig::set_axis_unit(Axis axis, Unit unit)
    {
        if (axis_to_unit_map_.count(axis) > 0)
        {
            axis_to_unit_map_[axis] = unit;
        }
    }


    double MotionConfig::axis_conversion(Axis axis)
    {
        double value = 1.0;
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            value = axis_to_unit_conversion_map_[axis];
        }
        return value;
    }


    void MotionConfig::set_axis_conversion(Axis axis, double value)
    {
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            axis_to_unit_conversion_map_[axis] = value;
        }
    }

    double MotionConfig::index_to_unit(Axis axis, int32_t index)
    {
        double scale_fact = 1.0;
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            scale_fact = axis_to_unit_conversion_map_[axis];
        }
        return scale_fact*index;
    }


    std::vector<double> MotionConfig::index_to_unit(std::vector<int32_t> index_vec)
    {
        std::vector<double> value_vec(index_vec.size());
        for (int i=0; i<index_vec.size(); i++)
        {
            value_vec[i] = index_to_unit(Axis(i),index_vec[i]);
        }
        return value_vec;
    }


    std::map<Axis,double> MotionConfig::index_to_unit(std::map<Axis,int32_t> index_map)
    {
        std::map<Axis,double> value_map;
        for (auto kv : index_map)
        {
            double value = index_to_unit(kv.first,kv.second);
            std::pair<Axis,double> kv_new(kv.first,value);
            value_map.insert(kv_new);
        }
        return value_map;
    }


    int32_t MotionConfig::unit_to_index(Axis axis, double  value)
    {
        double scale_fact = 1.0;
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            double scale_fact_inv = axis_to_unit_conversion_map_[axis];
            if (scale_fact_inv != 0)
            {
                scale_fact = 1.0/scale_fact_inv;
            }
        }
        return scale_fact*value;
    }


    std::vector<int32_t> MotionConfig::unit_to_index(std::vector<double> value_vec)
    {
        std::vector<int32_t> index_vec(value_vec.size());
        for (int i=0; i<value_vec.size(); i++)
        {
            index_vec[i] = unit_to_index(Axis(i),value_vec[i]);
        }
        return index_vec;
    }


    std::map<Axis,int32_t> MotionConfig::unit_to_index(std::map<Axis,double> value_map)
    {
        std::map<Axis,int32_t> index_map;
        for (auto kv : value_map)
        {
            int32_t index = unit_to_index(kv.first,kv.second);
            std::pair<Axis,int32_t> kv_new(kv.first,index);
            index_map.insert(kv_new);
        }
        return index_map;
    }


    // Utility functions
    // ----------------------------------------------------------------------------------
    std::string operating_mode_to_string(OperatingMode mode)
    {
        if (OperatingModeToStringMap.count(mode) > 0)
        {
            return OperatingModeToStringMap[mode];
        }
        else
        {
            return std::string("mode not found");
        }
    }


    std::string axis_to_string(Axis axis)
    {
        if (AxisToStringMap.count(axis) > 0)
        {
            return AxisToStringMap[axis];
        }
        else
        {
            return std::string("axis not found");
        }
    }


    std::string unit_to_string(Unit unit)
    {
        if (UnitToStringMap.count(unit) > 0)
        {
            return UnitToStringMap[unit];
        }
        else
        {
            return std::string("unit not found");
        }
    }
}
