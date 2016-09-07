#ifndef OUTSCAN_DATA_HPP
#define OUTSCAN_DATA_HPP

#include "rtn_status.hpp"
#include "rawhid_msg_types.h"
#include "motion_config.hpp"
#include <string>
#include <deque>
#include <armadillo>

namespace motion
{

    class OutscanData
    {
        public:

            OutscanData();
            arma::Col<double> time();
            arma::Mat<double> stepper_position();
            arma::Mat<double> pwm_position();
            arma::Mat<double> analog_input();
            arma::Mat<double> force_and_torque();
            arma::Col<uint8_t> status();
            arma::Col<uint8_t> count();
            arma::Col<uint8_t> command();
            arma::Col<uint16_t> command_data(); 

            Configuration config();
            void set_config(Configuration new_config);

            void update(DevToHostMsg msg);
            void clear();

            RtnStatus save(std::string filename);

        protected:

            std::deque<double> time_;
            std::deque<std::vector<double>> stepper_position_;
            std::deque<std::vector<double>> stepper_velocity_;
            std::deque<std::vector<double>> pwm_position_;
            std::deque<std::vector<double>> analog_input_;
            std::deque<uint8_t> status_;
            std::deque<uint8_t> count_;
            std::deque<uint8_t> command_;
            std::deque<uint16_t> command_data_;
            Configuration config_;
    };
}


#endif
