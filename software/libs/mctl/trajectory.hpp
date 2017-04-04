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

            std::string name();
            void set_name(std::string name);

            bool done();
            void set_done(bool done);

            arma::Row<double> start_pos();
            void set_start_pos(arma::Row<double> pos);

            double time(DevToHostMsg msg);

            virtual RtnStatus next(DevToHostMsg msg, Configuration &config, TrajectoryData &data_next);

        protected:

            bool done_ = false;
            bool is_first_ = true;
            std::string name_ = std::string("base");

            uint64_t start_time_us_ = 0;
            arma::Row<double> start_pos_;

    };


} // namespace mctl

#endif
