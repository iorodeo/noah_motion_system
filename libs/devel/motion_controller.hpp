#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include "rawhid_device.hpp"
#include "rawhid_msg_types.h"
#include "motion_constants.hpp"
#include "motion_config.hpp"
#include "rtn_status.hpp"
#include "outscan_data.hpp"

#include <string>
#include <vector>
#include <map>
#include <armadillo>

namespace motion
{

    class Controller
    {
        public:

            Controller(int vid=USB_VendorId, int pid=USB_ProductId);

            RtnStatus open();
            RtnStatus close();

            bool exit_on_error();
            void set_exit_on_error(bool value);

            bool display_position_on_move();
            void set_display_position_on_move(bool value);

            RtnStatus mode(OperatingMode &mode);  
            RtnStatus mode(std::string &mode_str);
            RtnStatus set_mode_ready();
            RtnStatus set_mode_disabled(); 
            RtnStatus stop_motion(bool wait=true, bool check=true);

            RtnStatus position(Axis axis, int32_t &ind);
            RtnStatus position(std::vector<int32_t> &ind_vec); 
            RtnStatus position(std::map<Axis,int32_t> &ind_map);
            RtnStatus position(arma::Row<int32_t> &ind_vec); 

            RtnStatus position(Axis axis, double  &pos);
            RtnStatus position(std::vector<double>  &pos_vec); 
            RtnStatus position(std::map<Axis,double>  &pos_map);
            RtnStatus position(arma::Row<double> &pos_vec);  

            RtnStatus print_position(bool unit=true);  

            void enable_homing(Axis axis);
            void disable_homing(Axis axis);
            bool is_homing_enabled(Axis axis);
            RtnStatus home(Axis axis, bool backoff=true, bool wait=true);
            RtnStatus set_homed_true(Axis axis);
            RtnStatus is_homed(Axis axis, bool &is_homed_flag);
            RtnStatus is_homed(std::map<Axis,bool> &is_homed_map);

            RtnStatus wait_for_ready(bool check=true, bool quiet=false);
            Unit axis_unit(Axis axis);
            std::string axis_unit_string(Axis axis);
            std::string axis_name(Axis axis);

            // Move to position methods for index arguments
            RtnStatus move_to_position(Axis axis, int32_t ind, bool wait=true);
            RtnStatus move_to_position(std::vector<int32_t> ind_vec, bool wait=true);
            RtnStatus move_to_position(std::map<Axis,int32_t> ind_map, bool wait=true);
            RtnStatus move_to_position(arma::Row<int32_t> ind_vec, bool wait=true); 

            // Move to position methods for unit arguments (m,deg)
            RtnStatus move_to_position(Axis axis, double pos, bool wait=true);
            RtnStatus move_to_position(std::vector<double> pos_vec, bool wait=true);
            RtnStatus move_to_position(std::map<Axis,double> pos_map, bool wait=true);
            RtnStatus move_to_position(arma::Row<double> pos_vec, bool wait=true); 

            // Jog position methods for index arguments
            RtnStatus jog_position(Axis axis, int32_t ind, bool wait=true);
            RtnStatus jog_position(std::vector<int32_t> ind_vec, bool wait=true);
            RtnStatus jog_position(std::map<Axis,int32_t> ind_map, bool wait=true);
            RtnStatus jog_position(arma::Row<int32_t> ind_vec, bool wait=true);  

            // Jog position methods for unit arguments
            RtnStatus jog_position(Axis axis, double pos, bool wait=true);
            RtnStatus jog_position(std::vector<double> pos_vec, bool wait=true);
            RtnStatus jog_position(std::map<Axis,double> pos_map, bool wait=true);
            RtnStatus jog_position(arma::Row<double> pos_vec, bool wait=true); 

            // Outscan methods 
            RtnStatus outscan(
                    arma::Mat<int32_t> ind_pos_mat, 
                    arma::Mat<int32_t> ind_vel_mat, 
                    OutscanData &data, 
                    bool quiet=false
                    ); 

            RtnStatus outscan(arma::Mat<double> pos_mat, OutscanData &data, bool quiet=false);   
            RtnStatus outscan(std::string filename, OutscanData &data, bool quiet=false);
            RtnStatus outscan(const char filename[], OutscanData &data, bool quiet=false);

            // Configuration methods
            Configuration config();
            //RtnStatus load_config(std::string filename);
            //RtnStatus set_config(MotionConfig config);

        protected:

            Configuration config_;
            RawHIDDevice hid_dev_;
            uint8_t msg_count_ = 0;
            bool exit_on_error_ = false;
            bool display_position_on_move_= false;

            RtnStatus send_command(HostToDevMsg &host_to_dev_msg, DevToHostMsg &dev_to_host_msg);
            RtnStatus check_status(RtnStatus rtn_status);
            RtnStatus set_mode_velocity_control();
    };

    // Utility functions
    // --------------------------------------------------------------------
    OperatingMode get_operating_mode(DevToHostMsg msg);
    std::vector<int32_t> get_index_position_std(DevToHostMsg msg);
    arma::Row<int32_t>   get_index_position_arma(DevToHostMsg msg);


} // namespace motion

// Using class method as callback
// --------------------------------------------------------------------------------------
//void run(std::function<void(int)> update);
//
// Calling class method
// --------------------------------------------------------------------------------------
//std::function<void(int)> func = std::bind(&Test::update, test, std::placeholders::_1);
//controller.run(func);
#endif
