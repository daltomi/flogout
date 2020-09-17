/*
	Copyright © 2020 daltomi <daltomi@disroot.org>

	This file is part of flogout.

	flogout is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	flogout is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with flogout.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "system.hpp"
#include <cassert>
#include <cstring>
#include <FL/fl_ask.H>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>


bool FileAccessOk(char const* const fileName, bool showError)
{
   assert(fileName);

   if (-1 == access(fileName, F_OK | R_OK | W_OK))
   {
     if (showError)
     {
       fl_alert("File: %s, error: %s", fileName, strerror(errno));
     }
     return false;
   }

   return true;
}
