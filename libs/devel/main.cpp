#include <iostream>
#include <functional>
#include "motion_controller.hpp"

const int Vid = 0x16C0;
const int Pid = 0x0486;

int main(int argc, char *argv[])
{
    MotionController controller(Vid,Pid);

    if (!controller.open())
    {
        std::cerr << "Error: unable to open motion controller" << std::endl;
    }

    controller.enable_homing(Axis_X);
    controller.enable_homing(Axis_Y);
    controller.enable_homing(Axis_Z);
    controller.enable_homing(Axis_A);



    controller.test();

    controller.close();
    return 0;
}

