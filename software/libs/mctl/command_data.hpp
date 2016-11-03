#ifndef COMMAND_DATA_HPP
#define COMMAND_DATA_HPP
#include <armadillo>
#include <cstdint>

namespace mctl
{
    struct CommandData
    { 
        arma::Row<int32_t> stepper_position; 
        arma::Row<int32_t> stepper_velocity;
        arma::Row<uint8_t> dio;
    };
}
#endif
