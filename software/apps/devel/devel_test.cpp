#include "controller.hpp"
#include "rtn_status.hpp"
#include "ft_sensor_cal.hpp"
#include "filesystem/path.h"

#include <iostream>
#include <fstream>


int main(int argc, char *argv[])
{
    mctl::FT_SensorCal cal;
    //cal.load("/home/wbd/.mctl/FT8652.cal");

    arma::Mat<double> v(100,10,arma::fill::zeros);
    arma::Mat<double> f;
    mctl::RtnStatus rtn_status = cal.convert(v,f);
    if (!rtn_status.success())
    {
        std::cout << "convert failed" << std::endl;
    }

    //mctl::Controller controller;

    //controller.set_exit_on_error(true);
    //controller.open();
    //controller.set_mode_ready();

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



