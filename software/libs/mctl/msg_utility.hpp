#ifndef MSG_UTILITY_HPP
#define MSG_UTILITY_HPP

#include "constants.hpp"

#include <vector>
#include <cstdint>
#include <armadillo>

namespace mctl
{
    struct DevToHostMsg;
    struct HostToDevMsg;

    OperatingMode get_operating_mode(DevToHostMsg msg);
    std::vector<int32_t> get_index_position_std(DevToHostMsg msg);
    arma::Row<int32_t>   get_index_position_arma(DevToHostMsg msg);
} 

#endif
