/* libtwit (C Twitter library)
* (c) Blake Smith
* Project homepage: http://github.com/blakesmith/libtwit/tree/master

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef LIBTWIT_STATUS_H
#define LIBTWIT_STATUS_H

#include "core.h"

int check_update_length(char *message);

struct tweet *parse_friends_timeline();

struct tweet *parse_user_timeline();

int send_update(char *message);
#endif
