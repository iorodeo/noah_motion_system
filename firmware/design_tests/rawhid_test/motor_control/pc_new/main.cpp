/*
Copyright 2016 IO Rodeo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <iostream>
#include <csignal>
#include <cstdlib>
#include "motion_controller.hpp"

bool quit_flag = false;
void sig_int_handler(int param);

int main(int argc, char *argv[])
{
    std::signal(SIGINT,sig_int_handler);

    MotionController controller(constants::Vid, constants::Pid);
    if (!controller.open()) 
    {
        exit(EXIT_FAILURE); 
    }
    controller.set_output_filename("data.txt");

    std::vector<TrajData> trajectory = get_cosine_traj(constants::Dt);
    controller.set_trajectory(trajectory);

    controller.run();
    controller.close();
    return 0;
}

void sig_int_handler(int param)
{
    quit_flag = true;
}
