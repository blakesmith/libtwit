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

int libtwit_check_update_length(char *message);

struct status *libtwit_parse_public_timeline();

struct status *libtwit_parse_friends_timeline(char *optional_parameters[][2], int param_length);

struct status *libtwit_parse_user_timeline(char *optional_parameters[][2], int param_length);

struct status *libtwit_parse_single_status(int id);

struct status *libtwit_status_update(char *message);

struct status *libtwit_get_recent_replies(char *optional_parameters[][2], int param_length);

struct status *libtwit_destroy_status(int id);
#endif
