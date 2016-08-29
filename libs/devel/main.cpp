#include <iostream>
#include <functional>
#include "motion_controller.hpp"

int main(int argc, char *argv[])
{
    motion::Controller controller;

    controller.set_exit_on_error(true);
    controller.open();
    controller.set_mode_ready();

    // Outscan examples - from double matrix
    // ----------------------------------------------------------------------------------
    for (auto num : motion::StepperList)
    { 
        controller.set_homed_true(num);
    }
    controller.set_display_position_on_move(true);
    controller.outscan("test_trajectory.txt");

    //arma::Mat<double> pos_mat = get_trajectory();
    //controller.outscan(pos_mat);

    // Move/jog examples
    // ----------------------------------------------------------------------------------
    //arma::Row<double> move_vec = {0.1,0,0,0,0};
    //controller.move_to_position(move_vec);

    //std::cout << "jogging" << std::endl;
    //arma::Row<double> jog_vec {0.0, 0.0, 0.0, 20.0 ,0};
    //controller.jog_position(jog_vec);
    
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
    //// --------------------------------------------------------------------------------

    controller.close();
    return 0;
}


//arma::Mat<double> get_trajectory()
//{
//    int num_reps = 2.0;
//    double period = 5.0;
//    double dt = motion::MessagePeriod_us*1.0e-6;
//    int num_pts = int(num_reps*period/dt);
//
//    arma::Row<double> amp_vec = {0.1,0.1,0.1,10.0,10.0};
//    arma::Row<double> ones(motion::NumStepper,arma::fill::ones);
//    arma::Mat<double> pos_mat(num_pts,motion::NumStepper);
//
//    double t = 0.0;
//    for (int i=0; i<num_pts; i++)
//    {
//        pos_mat.row(i) = amp_vec%arma::cos(2.0*arma::datum::pi*t*ones/period);
//        t += dt;
//    }
//    return pos_mat;
//}
//
//void get_trajectory_ind(arma::Mat<int32_t> &pos_ind, arma::Mat<int32_t> &vel_ind)
//{
//    int num_reps = 2.0;
//    double period = 5.0;
//    double dt = motion::MessagePeriod_us*1.0e-6;
//    double amp = 3000.0;
//    int num_pts = int(num_reps*period/dt);
//
//    pos_ind.resize(num_pts,motion::NumStepper);
//    vel_ind.resize(num_pts,motion::NumStepper);
//
//    arma::Row<double> ones(motion::NumStepper,arma::fill::ones);
//
//    double t = 0.0;
//    for (int i=0; i<num_pts; i++)
//    {
//        arma::Row<double> pos = amp*arma::cos(2.0*arma::datum::pi*t*ones/period);
//        arma::Row<double> vel = -(2.0*arma::datum::pi/period)*amp*arma::sin(2.0*arma::datum::pi*t*ones/period);
//
//        pos_ind.row(i) = arma::conv_to<arma::Mat<int32_t>>::from(pos);
//        vel_ind.row(i) = arma::conv_to<arma::Mat<int32_t>>::from(vel);
//
//        t += dt;
//    }
//}

