#ifndef MCONTROL_HPP
#define MCONTROL_HPP

#include "controller.hpp"
#include "rtn_status.hpp"
#include "docopt.h"
#include <map>
#include <string>

using mctl::RtnStatus;

typedef std::map<std::string,docopt::value> StringToValueMap;


static const char USAGE[] =
R"(
mcontrol  - Motion Controller 

    Usage:
      mcontrol home <axis> 
      mcontrol is-homed <axis>
      mcontrol set-homed <axis>
      mcontrol get-mode  
      mcontrol set-mode-ready
      mcontrol set-mode-disabled
      mcontrol move-to <axis> <value> 
      mcontrol move-to <X> <Y> <Z> <A> <B>
      mcontrol move-to-ind <axis> <value>
      mcontrol move-to-ind <X> <Y> <Z> <A> <B>
      mcontrol jog <axis> <value> 
      mcontrol jog <X> <Y> <Z> <A> <B>
      mcontrol jog-ind <axis> <value>
      mcontrol jog-ind <X> <Y> <Z> <A> <B>
      mcontrol outscan <input_file> [-o <output_file>] 
      mcontrol is-outscan-ready
      mcontrol get-position 
      mcontrol get-position-ind
      mcontrol set-position <axis> <value>
      mcontrol set-position <X> <Y> <Z> <A> <B>
      mcontrol set-position-ind <axis> <value>
      mcontrol set-position-ind <A> <Y> <Z> <A> <B> 
      mcontrol set-trigger-freq <trigger> <freq>
      mcontrol get-trigger-freq <trigger>
      mcontrol trigger-on <trigger>
      mcontrol trigger-off <trigger>
      mcontrol trigger-state <trigger>
      mcontrol get-ai-samples [-n <number>] [-o <output_file>]
      mcontrol get-ft-samples [-n <number>] [-o <output_file>]
      mcontrol ai-display 
      mcontrol ft-display
      mcontrol joystick
      mcontrol set-device-config
      mcontrol device-config
      mcontrol config-check
      mcontrol config-file
      mcontrol config-info
      mcontrol ft-info
      mcontrol status
      mcontrol help <command> 
      mcontrol (-h | --help)
      mcontrol (-v | --version)

    Options:
      -h --help          Show this screen.
      -o <output_file>   Specify output file 
      -n <number>        Number of samples
      -v --version       Show version.
)";

const std::string VersionString("mcontrol 0.1");

const int DisplayPositionPrecision = 4;

const int DisplayFrequencyPrecision = 2;

const std::string DefaultOutscanDataFile("outscan_data.h5");

const std::string DefaultSamplesDataFile("samples.txt");

std::map<std::string, mctl::Axis> StringToAxisMap = 
{
    {"x", mctl::Axis_X},
    {"y", mctl::Axis_Y},
    {"z", mctl::Axis_Z},
    {"a", mctl::Axis_A},
    {"b", mctl::Axis_B},
};

std::vector<std::string> AxisValueArgStringList = {"<X>", "<Y>", "<Z>", "<A>", "<B>"};

// Command functions
// -----------------------------------------------------------------------------------------------
bool cmd_home(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_is_homed(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_set_homed(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_get_mode(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_set_mode_ready(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_set_mode_disabled(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_move_to(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_move_to_ind(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_jog(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_jog_ind(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_outscan(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_is_outscan_ready(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_get_position(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_get_position_ind(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_set_position(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_set_position_ind(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_set_trigger_freq(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_get_trigger_freq(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_trigger_on(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_trigger_off(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_trigger_state(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_get_ai_samples(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_get_ft_samples(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_ai_display(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_ft_display(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_joystick(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_set_device_config(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_device_config(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_config_check(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_config_info(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_config_file(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_ft_info(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_status(mctl::Controller &controller, StringToValueMap arg_map);

bool cmd_help(mctl::Controller &controller, StringToValueMap arg_map);


// Utility functions
// ------------------------------------------------------------------------------------------------
StringToValueMap  get_arg_map(int argc, char *argv[]); 

void print_arg_map(StringToValueMap arg_map);

RtnStatus get_docopt_value_as_double(docopt::value docopt_value, double &value_dbl);

RtnStatus get_docopt_value_as_int32(docopt::value docopt_value, int32_t  &value_int32);

RtnStatus get_docopt_value_as_int(docopt::value docopt_value, int  &value_int);

RtnStatus get_axis_from_arg_map(StringToValueMap arg_map, mctl::Axis &axis);

RtnStatus get_axis_str_from_arg_map(StringToValueMap arg_map, std::string &axis_str);

RtnStatus get_trigger_from_docopt(docopt::value docopt_value, std::vector<int> &trigger_vec, bool allow_all=true);


// Command Map: command string ->  command function 
//-----------------------------------------------------------------------------------------------------
std::map<std::string,std::function<bool(mctl::Controller&,StringToValueMap)>> 
CmdStringToFuncMap = 
{
    {"home", &cmd_home},
    {"is-homed", &cmd_is_homed},
    {"set-homed", &cmd_set_homed},
    {"get-mode", &cmd_get_mode},
    {"set-mode-ready",   &cmd_set_mode_ready},
    {"set-mode-disabled", &cmd_set_mode_disabled},
    {"move-to", &cmd_move_to},
    {"move-to-ind", &cmd_move_to_ind},
    {"jog", &cmd_jog},
    {"jog-ind", &cmd_jog_ind},
    {"outscan", &cmd_outscan},
    {"is-outscan-ready", &cmd_is_outscan_ready},
    {"get-position", &cmd_get_position},
    {"get-position-ind", &cmd_get_position_ind},
    {"set-position", &cmd_set_position},           //
    {"set-position-ind", &cmd_set_position_ind},   //
    {"set-trigger-freq", &cmd_set_trigger_freq},
    {"get-trigger-freq", &cmd_get_trigger_freq},
    {"trigger-on", &cmd_trigger_on},
    {"trigger-off", &cmd_trigger_off},
    {"trigger-state", &cmd_trigger_state},
    {"get-ai-samples", &cmd_get_ai_samples},       //
    {"get-ft-samples", &cmd_get_ft_samples},       //
    {"ai-display", &cmd_ai_display},               //
    {"ft-display", &cmd_ft_display},               //
    {"joystick", &cmd_joystick},                   //
    {"set-device-config", &cmd_set_device_config},
    {"device-config", &cmd_device_config},
    {"config-check", &cmd_config_check},           //
    {"config-file", &cmd_config_file},
    {"config-info", &cmd_config_info},
    {"ft-info", &cmd_ft_info},
    {"status", &cmd_status},
    {"help", &cmd_help},
};


#endif
