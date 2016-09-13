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
#include <string>
#include <algorithm>
#include "motion_controller.hpp"
#include "dynamic_model.hpp"

bool quit_flag = false;
void sig_int_handler(int param);


int main(int argc, char *argv[])
{
    // Captive trajectory parameters (if enabled)
    const double mass = 0.5;
    const double damping = 1.0;
    //const double spring_const = 10.0;
    const double spring_const = 0.0;
    const double spring_zero = 0.0;
    const int captive_axis_num = 0;

    std::signal(SIGINT,sig_int_handler);

    // Check for 'captive' command line argument.
    std::vector<std::string> args(argv+1,argv+argc);
    bool enable_captive_axis = false;
    if (std::find(args.begin(), args.end(), "captive") != args.end())
    {
        enable_captive_axis = true;
    }
    int joystick_num = 0;
    if (std::find(args.begin(), args.end(), "js1") != args.end())
    {
        joystick_num = 1;
    }

    // Setup up motion controller
    MotionController controller(constants::Vid, constants::Pid, joystick_num);
    if (!controller.open()) 
    {
        exit(EXIT_FAILURE); 
    }
    controller.set_output_filename("data.txt");

    // Create motor trajectory and pass it to controller 
    //std::vector<TrajData> trajectory = get_multicosine_traj(constants::Dt);
    std::vector<TrajData> trajectory = get_cosine_traj(constants::Dt);
    controller.set_trajectory(trajectory);

    // Setup captive trajectory
    if (enable_captive_axis)
    {
        DynamicModel dynamic_model(mass,damping,spring_const,spring_zero);
        controller.enable_captive_trajectory(captive_axis_num,dynamic_model);
    }

    // Run controller 
    controller.run();
    controller.close();
    return 0;
}

void sig_int_handler(int param)
{
    quit_flag = true;
}
