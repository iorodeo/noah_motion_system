#include <iostream>
#include "motion_controller.hpp"

int main(int argc, char *argv[])
{
    motion::OutscanData data;

    arma::Mat<double> time = data.time();
    std::cout << time;


    //data.save(std::string("test_outscan_data.h5"));


    //motion::Controller controller;

    //controller.set_exit_on_error(true);
    //controller.open();
    //controller.set_mode_ready();

    //for (auto num : motion::StepperList)
    //{ 
    //    controller.set_homed_true(num);
    //}
    //controller.set_display_position_on_move(true);

    //motion::OutscanData outscan_data;
    //controller.outscan("test_trajectory.txt",outscan_data);

    //controller.close();
    return 0;
}



