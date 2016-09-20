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

----------------------------------------------------------------------- */
#include "util.h"
#include <stdio.h>


void print_err_msg(const char *file, int line, const char *func, char *err_msg)
{
    fprintf(stderr, "%s:%d, %s, Error: %s\n",file, line, func, err_msg);
    return;
}

