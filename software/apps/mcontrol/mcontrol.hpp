#ifndef CMD_LINE_APP_HPP
#define CMD_LINE_APP_HPP

#include "controller.hpp"
#include "rtn_status.hpp"
#include "docopt.h"
#include <map>
#include <string>

using mctl::RtnStatus;

const int DisplayPositionPrecision = 4;

static const char USAGE[] =
R"(
mctl  - Motion Controller 

    Usage:
      mctl home <axis> 
      mctl is-homed <axis>
      mctl set-homed <axis>
      mctl get-mode  
      mctl set-mode-ready
      mctl set-mode-disabled
      mctl move-to <axis> <value> 
      mctl move-to <X> <Y> <Z> <A> <B>
      mctl move-to-ind <axis> <value>
      mctl move-to-ind <X> <Y> <Z> <A> <B>
      mctl jog <axis> <value> 
      mctl jog <X> <Y> <Z> <A> <B>
      mctl jog-ind <axis> <value>
      mctl jog-ind <X> <Y> <Z> <A> <B>
      mctl outscan <input_file> [-o <output_file>] 
      mctl get-position 
      mctl get-position-ind
      mctl get-config
      mctl ft-info
      mctl status
      mctl help <command> 
      mctl (-h | --help)
      mctl (-v | --version)

    Options:
      -h --help     Show this screen.
      -v --version  Show version.
      -o <output_file>     Specify output file [default: outscan_data.h5]
)";


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
void cmd_home(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_is_homed(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_set_homed(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_get_mode(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_set_mode_ready(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_set_mode_disabled(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_get_position(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_get_position_ind(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_move_to(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_move_to_ind(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_jog(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_jog_ind(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_outscan(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_status(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_ft_info(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);

void cmd_help(mctl::Controller &controller, std::map<std::string,docopt::value> arg_map);


// Utility functions
// ------------------------------------------------------------------------------------------------
std::map<std::string,docopt::value>  get_arg_map(int argc, char *argv[]); 

void print_arg_map(std::map<std::string,docopt::value> arg_map);

RtnStatus get_docopt_value_as_double(docopt::value docopt_value, double &value_dbl);

RtnStatus get_docopt_value_as_int32(docopt::value docopt_value, int32_t  &value_int32);

RtnStatus get_axis_from_arg_map(std::map<std::string,docopt::value> arg_map, mctl::Axis &axis);

RtnStatus get_axis_str_from_arg_map(std::map<std::string,docopt::value> arg_map, std::string &axis_str);


// Command Map: command string ->  command function 
//-----------------------------------------------------------------------------------------------------
std::map<std::string,std::function<void(mctl::Controller&,std::map<std::string,docopt::value>)>> 
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
    {"ft-info", &cmd_ft_info},
    {"status", &cmd_status},
    {"help", &cmd_help},
};
#endif
