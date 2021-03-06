#include <mctl/mctl.hpp>
#include <mctl/joystick.hpp>
#include <iostream>
#include "joy_trajectory.hpp"

int main(int argc, char *argv[])
{
    // Create and setup controller
    mctl::Controller controller;
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

    // Setup joystick device
    Joystick joystick = controller.config().joystick_device();
    if (!joystick.isFound())
    {
        std::cerr << "error: unable to find joystick" << std::endl;
        exit(0);
    }

    // Create trajectory `
    JoyTrajectory trajectory = JoyTrajectory(joystick);

    // Run captive trajectory
    mctl::OutscanData data;
    controller.run_trajectory(trajectory,data,false);

    // Clean up and save data
    controller.close();
    data.save("data.h5");

    return 0;
}
