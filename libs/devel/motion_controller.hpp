#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include "rawhid_device.hpp"
#include "rawhid_msg_types.h"
#include "motion_constants.hpp"
#include "motion_config.hpp"
#include "rtn_status.hpp"

#include <string>
#include <vector>
#include <map>

namespace motion
{
    class Controller
    {
        public:

            Controller(int vid=USB_VendorId, int pid=USB_ProductId);

            RtnStatus open();
            RtnStatus close();

            RtnStatus position(Axis axis, int32_t &ind);
            RtnStatus position(Axis axis, double  &pos);
            RtnStatus position(std::vector<int32_t> &ind_vec); 
            RtnStatus position(std::vector<double>  &pos_vec); 
            RtnStatus position(std::map<Axis,int32_t> &ind_map);
            RtnStatus position(std::map<Axis,double>  &pos_map);
            RtnStatus print_position(bool unit=true);  

            RtnStatus set_mode_ready();
            RtnStatus set_mode_disabled();

            void enable_homing(Axis axis);
            void disable_homing(Axis axis);
            bool is_homing_enabled(Axis axis);

            RtnStatus home(Axis axis, bool wait=true);
            RtnStatus set_homed_true(Axis axis);

            RtnStatus wait_for_ready();
            Unit axis_unit(Axis axis);

            // Move to position methods for index arguments
            RtnStatus move_to_position(Axis axis, int32_t ind, bool wait=true);
            RtnStatus move_to_position(std::vector<int32_t> ind_vec, bool wait=true);
            RtnStatus move_to_position(std::map<Axis,int32_t> ind_map, bool wait=true);

            // Move to position methods for unit arguments (m,deg)
            RtnStatus move_to_position(Axis axis, double pos, bool wait=true);
            RtnStatus move_to_position(std::vector<double> pos_vec, bool wait=true);
            RtnStatus move_to_position(std::map<Axis,double> pos_map, bool wait=true);

            // NOT DONE
            // ------------------------------------------------------------------
            RtnStatus jog_position(Axis axis, int32_t ind, bool wait=true);
            RtnStatus jog_position(std::vector<int32_t> ind_vec, bool wait=true);
            RtnStatus jog_position(std::map<Axis,int32_t> ind_map, bool wait=true);

            RtnStatus jog_position(Axis axis, double pos, bool wait=true);
            RtnStatus jog_position(std::vector<double> pos_vec, bool wait=true);
            RtnStatus jog_position(std::map<Axis,double> pos_map, bool wait=true);
            // -------------------------------------------------------------------

            //MotionConfig config();
            //RtnStatus load_config(std::string filename);
            //RtnStatus set_config(MotionConfig config);
            //RtnStatus outscan_trajectory();

            // ----------------------------------------------------------
            //
            void test();

        protected:

            MotionConfig config_;

            RawHIDDevice hid_dev_;
            uint8_t msg_count_ = 0;

            RtnStatus send_command(HostToDevMsg &host_to_dev_msg, DevToHostMsg &dev_to_host_msg);

    };

    // Utility functions
    // --------------------------------------------------------------------
    OperatingMode get_operating_mode(DevToHostMsg msg);


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
