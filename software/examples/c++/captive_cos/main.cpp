#include <mctl/mctl.hpp>
#include <iostream>
#include "cos_trajectory.hpp"

int main(int argc, char *argv[])
{
    mctl::Controller controller;
    CosTrajectory trajectory;

    // Setup controller
    controller.set_exit_on_error(true);
    controller.load_config();
    controller.open();
    controller.set_mode_ready();
    controller.set_display_position_on_move(true);

    // Check that controller is ready to run trajectory 
    bool ok;
    controller.is_ready_for_run(ok);
    if (!ok)
    {
        std::cout << "error: ready for outscan - check mode and homing" << std::endl;
        exit(0);
    }

    // Run captive trajectory
    mctl::OutscanData data;
    controller.run_trajectory(trajectory,data);

    controller.close();

    return 0;
}
