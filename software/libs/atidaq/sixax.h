/*---------------------------------------------------------------------
  sixaxff
  Copyright (C) William Dickson, 2008.
  
  wbd@caltech.edu
  www.willdickson.com

  Released under the LGPL Licence, Version 3
  
  This file is part of sixaxff.

  sixaxff is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.
    
  sixaxff is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
  Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with sixaxff.  If not, see
  <http://www.gnu.org/licenses/>.

----------------------------------------------------------------------
  sixax.h

  Contains code for initializing the 6 axis sensor calibration and
  converting from samples to forces and torques

  Author: Will Dickson
---------------------------------------------------------------------- */
#ifndef INC_FTCONVERT_H_
#define INC_FTCONVERT_H_
#include <stdio.h>
#include <stdlib.h>
#include "ftconfig.h"


extern int sixax_init_cal(Calibration **cal, char *cal_file_path, float tooltrans[]); 
extern int sixax_set_bias(Calibration *cal, float bias[]); 
extern int sixax_set_tooltrans(Calibration *cal, float tooltrans[]);
extern int sixax_sample2ft(Calibration *cal, float sample[], float ft[]); 
extern void sixax_print_calinfo(Calibration *cal);
extern void sixax_free_cal(Calibration *cal);
extern void sixax_read_calinfo(char *cal_file_path); 

#endif // INC_FTCONVERT_H_
