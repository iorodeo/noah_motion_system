#include <iostream>
#include "controller.hpp"
#include "configuration.hpp"
#include "ft_sensor_cal.hpp"
#include "ft_tool_transform.hpp"

int main(int argc, char *argv[])
{

    mctl::FT_SensorCal ft_sensor_cal;
    mctl::RtnStatus rtn_status = ft_sensor_cal.set_from_file("FT8652.cal");

    ft_sensor_cal.print_info_string();





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



