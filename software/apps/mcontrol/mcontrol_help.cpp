#include "mcontrol_help.hpp"
#include <iostream>
#include <sstream>

typedef std::map<std::string, std::string> StringMap;


const std::string home_help_string(R"( 
mcontrol home <axis> 

Initiates homing move for the selected <axis>. 
)");

const std::string is_homed_help_string(R"( 
mcontrol is-homed <axis>
)");


const std::string set_homed_help_string(R"( 
mcontrol set-homed <axis>
)");


const std::string get_mode_help_string(R"( 
mcontrol get-mode  
)");


const std::string set_mode_ready_help_string(R"( 
mcontrol set-mode-ready
)");


const std::string set_mode_disabled_help_string(R"( 
mcontrol set-mode-disabled
)");


const std::string move_to_help_string(R"( 
mcontrol move-to <axis> <value> 
mcontrol move-to <X> <Y> <Z> <A> <B>
)");


const std::string move_to_ind_help_string(R"( 
mcontrol move-to-ind <axis> <value>
mcontrol move-to-ind <X> <Y> <Z> <A> <B>
)");


const std::string jog_help_string(R"( 
mcontrol jog <axis> <value> 
mcontrol jog <X> <Y> <Z> <A> <B>
)");


const std::string jog_ind_help_string(R"( 
mcontrol jog-ind <axis> <value>
mcontrol jog-ind <X> <Y> <Z> <A> <B>
)");


const std::string outscan_help_string(R"( 
mcontrol outscan <input_file> [-o <output_file>] 
)");


const std::string get_position_help_string(R"( 
mcontrol get-position 
)");

    
const std::string get_position_ind_help_string(R"( 
mcontrol get-position-ind 
)");


const std::string config_info_help_string(R"( 
mcontrol config
)");


const std::string config_file_help_string(R"( 
mcontrol config
)");


const std::string ft_info_help_string(R"( 
mcontrol ft-info
)");


const std::string status_help_string(R"( 
mcontrol status
)");


const std::string help_help_string(R"( 
mcontrol help 
)");


const StringMap CommandToHelpMap = 
{
    {"home", home_help_string}, 
    {"is-homed", is_homed_help_string}, 
    {"set-homed", set_homed_help_string},
    {"get-mode", get_mode_help_string}, 
    {"set-mode-ready", set_mode_ready_help_string},
    {"set-mode-disabled", set_mode_disabled_help_string},
    {"move-to", move_to_help_string},
    {"move-to-ind", move_to_ind_help_string},
    {"jog", jog_help_string}, 
    {"jog-ind", jog_ind_help_string},
    {"outscan", outscan_help_string}, 
    {"get-position", get_position_help_string}, 
    {"get-position-ind", get_position_ind_help_string},
    {"config-file", config_file_help_string},
    {"config-info", config_info_help_string},
    {"ft-info", ft_info_help_string},
    {"status", status_help_string},
    {"help", help_help_string},
};


mctl::RtnStatus print_command_help(std::string command)
{
    mctl::RtnStatus rtn_status;
    if (CommandToHelpMap.count(command)==0)
    {
        std::ostringstream oss;
        oss << "error: command " << command < " not found";
        rtn_status.set_success(false);
        rtn_status.set_error_msg(oss.str());
        return rtn_status;
    }
    std::cout << CommandToHelpMap.at(command);
    std::cout << std::endl;
    return rtn_status;
}
