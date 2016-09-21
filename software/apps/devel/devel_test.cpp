#include <iostream>
#include "controller.hpp"
#include "configuration.hpp"
#include "ft_sensor_cal.hpp"
#include "ft_tool_transform.hpp"

int main(int argc, char *argv[])
{

    motion::FT_SensorCal ft_sensor_cal;
    motion::RtnStatus rtn_status = ft_sensor_cal.set_from_file("FT8652.cal");
    std::cout << "success      = " << (rtn_status.success()) << std::endl;

    std::string force_units;
    ft_sensor_cal.get_force_units(force_units);

    std::string torque_units;
    ft_sensor_cal.get_torque_units(torque_units);

    motion::FT_ToolTransform trans;
    ft_sensor_cal.get_tool_transform(trans);

    std::cout << "force units  = " << force_units << std::endl;
    std::cout << "torque units = " << torque_units << std::endl;
    std::cout << "trans position units = " << (trans.position_units()) << std::endl;
    std::cout << "trans rotation units = " << (trans.rotation_units()) << std::endl;


    //motion::Controller controller;

    //controller.set_exit_on_error(true);
    //controller.open();
    //controller.set_mode_ready();

    //for (auto num : motion::StepperList)
    //{ 
    //    controller.set_homed_true(num);
    //}
    //controller.set_display_position_on_move(true);

    //motion::OutscanData data;
    //controller.outscan("test_trajectory.txt", data);

    //data.save("test_data.h5");

    //controller.close();
    return 0;
}



