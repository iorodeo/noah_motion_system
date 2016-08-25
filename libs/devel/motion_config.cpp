#include "motion_config.hpp"
#include <cmath>

namespace motion
{
    // Configuration public methods
    // ----------------------------------------------------------------------------------
    Configuration::Configuration()
    {
        homing_enabled_map_ = DefaultHomingEnabledMap;
        homing_backoff_map_ = DefaultHomingBackoffMap; 
        axis_to_unit_map_ = DefaultAxisToUnitMap;
        axis_to_unit_conversion_map_ = DefaultAxisToUnitConversionMap;
        outscan_start_delay_ = DefaultOutscanStartDelay_ms;
        gain_ = DefaultGain;

    }


    // Configuration protected methods
    // ----------------------------------------------------------------------------------
    bool Configuration::homing_enabled(Axis axis)
    {
        bool rval = false;
        if (homing_enabled_map_.count(axis) > 0)
        {
            rval = homing_enabled_map_[axis];
        }
        return rval;
    }


    void Configuration::set_homing_enabled(Axis axis, bool value)
    {
        if (homing_enabled_map_.count(axis) > 0)
        {
            homing_enabled_map_[axis] = value;
        }
    }


    Unit Configuration::axis_unit(Axis axis)
    {
        Unit unit = NoUnit;
        if (axis_to_unit_map_.count(axis) > 0)
        {
            unit = axis_to_unit_map_[axis];
        }
        return unit;
    }

    void Configuration::set_axis_unit(Axis axis, Unit unit)
    {
        if (axis_to_unit_map_.count(axis) > 0)
        {
            axis_to_unit_map_[axis] = unit;
        }
    }


    double Configuration::axis_conversion(Axis axis)
    {
        double value = 1.0;
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            value = axis_to_unit_conversion_map_[axis];
        }
        return value;
    }


    void Configuration::set_axis_conversion(Axis axis, double value)
    {
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            axis_to_unit_conversion_map_[axis] = value;
        }
    }

    double Configuration::index_to_unit(Axis axis, int32_t index)
    {
        double scale_fact = 1.0;
        if (axis_to_unit_conversion_map_.count(axis) > 0)
        {
            scale_fact = axis_to_unit_conversion_map_[axis];
        }
        return scale_fact*index;
    }


    std::vector<double> Configuration::index_to_unit(std::vector<int32_t> index_vec)
    {
        std::vector<double> value_vec(index_vec.size());
        for (int i=0; i<index_vec.size(); i++)
        {
            value_vec[i] = index_to_unit(Axis(i),index_vec[i]);
        }
        return value_vec;
    }


    std::map<Axis,double> Configuration::index_to_unit(std::map<Axis,int32_t> index_map)
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

    arma::Row<double> Configuration::index_to_unit(arma::Row<int32_t> index_vec)
    {
        arma::Row<double> value_vec(index_vec.size());
        for (int i=0; i<index_vec.size(); i++)
        {
            value_vec(i) = index_to_unit(Axis(i),index_vec(i));
        }
        return value_vec;
    }


    int32_t Configuration::unit_to_index(Axis axis, double  value)
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


    std::vector<int32_t> Configuration::unit_to_index(std::vector<double> value_vec)
    {
        std::vector<int32_t> index_vec(value_vec.size());
        for (int i=0; i<value_vec.size(); i++)
        {
            index_vec[i] = unit_to_index(Axis(i),value_vec[i]);
        }
        return index_vec;
    }


    std::map<Axis,int32_t> Configuration::unit_to_index(std::map<Axis,double> value_map)
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

    arma::Row<int32_t> Configuration::unit_to_index(arma::Row<double> index_vec)
    {
        arma::Row<int32_t> value_vec(index_vec.size());
        for (int i=0; i<index_vec.size(); i++)
        {
            value_vec(i) = unit_to_index(Axis(i),index_vec(i));
        }
        return value_vec;
    }


    double Configuration::homing_backoff(Axis axis)
    {
        double backoff = 0.0;
        double direction = 1.0;

        if (homing_backoff_map_.count(axis) > 0)
        {
            backoff = homing_backoff_map_[axis];
        }

        if (axis < NumStepper)
        {
            if (HomingDirection[axis] > 0)
            {
                direction = -1.0;
            }
        }
        return direction*backoff;
    }


    void Configuration::set_homing_backoff(Axis axis, double value)
    {
        if (homing_backoff_map_.count(axis) > 0)
        {
            homing_backoff_map_[axis] = fabs(value);
        }
    }


    int Configuration::outscan_start_delay()
    {
        return outscan_start_delay_;
    }


    void Configuration::set_outscan_start_delay(int value)
    {
        outscan_start_delay_ = value;
    }

    int32_t Configuration::gain()
    {
        return gain_;
    }

    void Configuration::set_gain(int32_t gain)
    {
        gain_ = gain;
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
