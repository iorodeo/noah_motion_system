#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP
#include "constants.hpp"
#include "rtn_status.hpp"
#include "configuration.hpp"
#include "trajectory_data.hpp"

#include <string>
#include <armadillo>

namespace mctl
{
    struct DevToHostMsg;

    class Trajectory
    {

        // Base class for captive trajectories

        public:

            Trajectory();

            void initialize();
            void initialize(Configuration config);

            Configuration config();
            void set_config(Configuration config);

            std::string name();
            void set_name(std::string name);

            bool done();
            void set_done(bool done);

            arma::Row<double> position();
            void set_position(arma::Row<double> pos);

            arma::Row<double> velocity();
            void set_velocity(arma::Row<double> vel);
            
            arma::Row<double> start_position();
            void set_start_position(arma::Row<double> pos);
            
            arma::Row<uint8_t> digital_output();
            void set_digital_output(arma::Row<uint8_t> dout);

            virtual RtnStatus update(DevToHostMsg msg);
            virtual arma::Row<double> force_and_torque(DevToHostMsg msg);
            virtual arma::Row<double> analog_input(DevToHostMsg msg);

            double time(DevToHostMsg msg);
            

        protected:

            std::string name_ = std::string("base");
            arma::Row<double> start_pos_;
            arma::Row<double> pos_;
            arma::Row<double> vel_;
            arma::Row<uint8_t> dout_;

            bool done_ = false;
            bool is_first_ = true;
            uint64_t start_time_us_ = 0;
            Configuration config_;

    };


} // namespace mctl

#endif
