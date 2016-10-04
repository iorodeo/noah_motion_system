#include "controller.hpp"
#include "rtn_status.hpp"
#include "ft_sensor_cal.hpp"
#include "filesystem/path.h"

#include <iostream>
#include <fstream>


int main(int argc, char *argv[])
{
    mctl::Controller controller;

    controller.set_exit_on_error(true);
    controller.load_config();

    controller.open();
    controller.set_mode_ready();

    controller.ft_display();

    //arma::Mat<double> data;
    //controller.get_ft_samples(data,100);
    //std::cout << data;

    //controller.joystick_pendant();

    //for (auto num : mctl::StepperList)
    //{ 
    //    controller.set_homed_true(num);
    //}
    //controller.set_display_position_on_move(true);

    //mctl::OutscanData data;
    //controller.outscan("test_trajectory.txt", data);

    //data.save("test_data.h5");

    //controller.close();
    return 0;
}



