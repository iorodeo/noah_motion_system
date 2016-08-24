#include <iostream>
#include <functional>
#include "motion_controller.hpp"

int main(int argc, char *argv[])
{
    motion::Controller controller;
    controller.set_exit_on_error(true);
    controller.open();
    controller.set_mode_ready();
    
    //// Get mode and print 
    //std::string mode_str;
    //controller.mode(mode_str);
    //std::cout << "mode: " << mode_str << std::endl << std::endl;

    //// Get current postion
    //arma::Row<double> pos_vec;
    //controller.position(pos_vec);
    //std::cout << "postion: " << pos_vec << std::endl;

    // Move to position example
    // ----------------------------------------------------------------------------------
    for (auto num : motion::StepperList)
    { 
        controller.set_homed_true(num);
    }
    controller.set_display_position_on_move(true);

    //arma::Row<double> move_vec = {0.1,0,0,0,0};
    //controller.move_to_position(move_vec);

    std::cout << "jogging" << std::endl;
    arma::Row<double> jog_vec {0.0, 0.0, 0.0, 20.0 ,0};
    controller.jog_position(jog_vec);
    
    //arma::Row<double> move_vec {0.0, 0.0, 0.0, 0.0 ,0};
    //controller.move_to_position(move_vec);
    //std::cout << std::endl;

    //// Get current postion
    //controller.position(pos_vec);
    //std::cout << "postion: " << pos_vec << std::endl; 

    //// Homing example
    //// ----------------------------------------------------------------------------------
    //std::cout << std::endl;
    //for (auto num : motion::StepperList)
    //{
    //    std::string axis_name = motion::axis_to_string(num);
    //    std::cout << "* homing " << axis_name << " ..." << std::flush;
    //    controller.home(num);
    //    std::cout << " done" << std::endl;
    //// ------------------------------------------------------------------------

    controller.close();
    return 0;
}

