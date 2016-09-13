#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP
#include <vector>
#include "constants.h"

struct TrajData
{
    double pos[constants::NumMotor];
    double vel[constants::NumMotor];
};

std::vector<TrajData> get_cosine_traj(double dt);
std::vector<TrajData> get_multicosine_traj(double dt);

#endif
