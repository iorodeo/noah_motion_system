#ifndef MOTION_CONFIG_HPP
#define MOTION_CONFIG_HPP

#include "motion_constants.hpp"

#include <map>
#include <vector>
#include <armadillo>

namespace motion
{
    class Configuration
    {
        public:
            Configuration();

            bool homing_enabled(Axis axis);
            void set_homing_enabled(Axis axis, bool value);

            Unit axis_unit(Axis axis);
            void set_axis_unit(Axis axis, Unit unit);
            std::string axis_unit_string(Axis axis);
            std::string axis_name(Axis axis);

            double index_to_unit(Axis axis, int32_t index);
            std::vector<double> index_to_unit(std::vector<int32_t> index_vec);
            std::map<Axis,double> index_to_unit(std::map<Axis,int32_t> index_map);
            arma::Row<double> index_to_unit(arma::Row<int32_t> index_vec);
            arma::Mat<double> index_to_unit(arma::Mat<int32_t> index_mat);

            int32_t unit_to_index(Axis axis, double  value);
            std::vector<int32_t> unit_to_index(std::vector<double> value_vec);
            std::map<Axis,int32_t> unit_to_index(std::map<Axis,double> value_map);
            arma::Row<int32_t> unit_to_index(arma::Row<double> value_vec);
            arma::Mat<int32_t> unit_to_index(arma::Mat<double> value_mat);

            double axis_conversion(Axis axis);
            double axis_conversion_inv(Axis axis);
            void set_axis_conversion(Axis axis, double value);

            double homing_backoff(Axis axis);
            void set_homing_backoff(Axis axis, double value);

            int outscan_start_delay();
            void set_outscan_start_delay(int value);

            int32_t gain();
            void set_gain(int32_t gain);

        protected:
            std::map<Axis,bool> homing_enabled_map_;
            std::map<Axis,Unit> axis_to_unit_map_;
            std::map<Axis,double> axis_to_unit_conversion_map_; 
            std::map<Axis,double> homing_backoff_map_;
            int outscan_start_delay_;
            int32_t gain_;
    };

    // Utility functions
    // ----------------------------------------------------------
    std::string operating_mode_to_string(OperatingMode mode);
    std::string axis_to_string(Axis axis);
    std::string unit_to_string(Unit unit);

}

#endif
