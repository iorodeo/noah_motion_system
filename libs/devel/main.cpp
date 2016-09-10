#include <iostream>
#include "motion_controller.hpp"

int main(int argc, char *argv[])
{
    motion::Controller controller;

    controller.set_exit_on_error(true);
    controller.open();
    controller.set_mode_ready();

    for (auto num : motion::StepperList)
    { 
        controller.set_homed_true(num);
    }
    controller.set_display_position_on_move(true);

    motion::OutscanData data;
    controller.outscan("test_trajectory.txt", data, true);

    data.save("test_data.h5");

    controller.close();
    return 0;
}



