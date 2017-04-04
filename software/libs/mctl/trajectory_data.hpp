#ifndef TRAJECTORY_DATA_HPP
#define TRAJECTORY_DATA_HPP

#include <armadillo>

namespace mctl
{

    class TrajectoryData
    {
        public:

            TrajectoryData()
            { 
                pos = arma::Row<double>(NumStepper,arma::fill::zeros);
                vel = arma::Row<double>(NumStepper,arma::fill::zeros);
                dio = arma::Row<uint8_t>(NumDigitalOutput,arma::fill::zeros);
            };
            arma::Row<double>  pos;
            arma::Row<double>  vel;
            arma::Row<uint8_t> dio;
    };


} // namespace mctl

#endif
