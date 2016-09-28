#ifndef MCONTROL_HPP
#define MCONTROL_HPP

#include "controller.hpp"
#include "rtn_status.hpp"
#include "docopt.h"
#include <map>
#include <string>

using mctl::RtnStatus;

typedef std::map<std::string,docopt::value> StringToValueMap;

const int DisplayPositionPrecision = 4;

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
      mcontrol get-position 
      mcontrol get-position-ind
      mcontrol config
      mcontrol ft-info
      mcontrol status
      mcontrol help <command> 
      mcontrol (-h | --help)
      mcontrol (-v | --version)

    Options:
      -h --help     Show this screen.
      -v --version  Show version.
      -o <output_file>     Specify output file [default: outscan_data.h5]
)";

const std::string VersionString("mcontrol 0.1");

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
void cmd_home(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_is_homed(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_set_homed(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_get_mode(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_set_mode_ready(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_set_mode_disabled(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_get_position(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_get_position_ind(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_move_to(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_move_to_ind(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_jog(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_jog_ind(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_outscan(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_status(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_config(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_ft_info(mctl::Controller &controller, StringToValueMap arg_map);

void cmd_help(mctl::Controller &controller, StringToValueMap arg_map);


// Utility functions
// ------------------------------------------------------------------------------------------------
StringToValueMap  get_arg_map(int argc, char *argv[]); 

void print_arg_map(StringToValueMap arg_map);

RtnStatus get_docopt_value_as_double(docopt::value docopt_value, double &value_dbl);

RtnStatus get_docopt_value_as_int32(docopt::value docopt_value, int32_t  &value_int32);

RtnStatus get_axis_from_arg_map(StringToValueMap arg_map, mctl::Axis &axis);

RtnStatus get_axis_str_from_arg_map(StringToValueMap arg_map, std::string &axis_str);


// Command Map: command string ->  command function 
//-----------------------------------------------------------------------------------------------------
std::map<std::string,std::function<void(mctl::Controller&,StringToValueMap)>> 
CmdStringToFuncMap = 
{
    {"home", &cmd_home},
    {"is-homed", &cmd_is_homed},
    {"set-homed", &cmd_set_homed},
    {"get-mode", &cmd_get_mode},
    {"set-mode-ready",   &cmd_set_mode_ready},
    {"set-mode-disabled", &cmd_set_mode_disabled},
    {"get-position", &cmd_get_position},
    {"get-position-ind", &cmd_get_position_ind},
    {"move-to", &cmd_move_to},
    {"move-to-ind", &cmd_move_to_ind},
    {"jog", &cmd_jog},
    {"jog-ind", &cmd_jog_ind},
    {"outscan", &cmd_outscan},
    {"config",  &cmd_config},
    {"ft-info", &cmd_ft_info},
    {"status", &cmd_status},
    {"help", &cmd_help},
};

#endif
