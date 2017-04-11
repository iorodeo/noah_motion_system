#include "msg_utility.hpp"
#include "rawhid_msg_types.h"

namespace mctl
{
    OperatingMode get_operating_mode(DevToHostMsg msg)
    {
        OperatingMode mode = OperatingMode(msg.status & ModeBitsMask);
        return mode;
    }

    std::vector<int32_t> get_index_position_std(DevToHostMsg msg)
    {
        std::vector<int32_t> ind_vec(NumStepper);
        for (auto num : StepperList)
        {
            ind_vec[num] = msg.stepper_position[num];
        }
        return ind_vec;
    }
    
    arma::Row<int32_t> get_index_position_arma(DevToHostMsg msg)
    {
        arma::Row<int32_t> ind_vec(NumStepper);
        for (auto num : StepperList)
        {
            ind_vec(num) = msg.stepper_position[num];
        }
        return ind_vec;
    }

} // namespace mctl 
