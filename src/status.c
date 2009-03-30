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

#include "status.h"
#include "core.h"

#include <string.h>

int 
libtwit_check_update_length(char *message)
{
	if (strlen(message) > 140)
		return LIBTWIT_MESSAGE_TOO_LONG;
	else
		return LIBTWIT_OK;
}

struct status
*libtwit_parse_public_timeline()
{
	struct status *public_timeline = parse_status_doc(GET_REQUEST, STATUS_URL, PUBLIC_TIMELINE, NULL, 0);

	return public_timeline;
}

struct status 
*libtwit_parse_friends_timeline(char *optional_parameters[][2], int param_length)
{
	struct status *user_timeline = parse_status_doc(GET_REQUEST, STATUS_URL, FRIENDS_TIMELINE, optional_parameters, param_length);
	
	return user_timeline;
}

struct status 
*libtwit_parse_user_timeline(char *optional_parameters[][2], int param_length)
{
	struct status *user_timeline = parse_status_doc(GET_REQUEST, STATUS_URL, USER_TIMELINE, optional_parameters, param_length);
	
	return user_timeline;
}

struct status
*libtwit_parse_single_status(int id)
{
	char single_status_url[SLENGTH];
	sprintf(single_status_url, "%s%i.xml", SINGLE_STATUS, id);

	struct status *single_status = parse_status_doc(GET_REQUEST, STATUS_URL, single_status_url, NULL, 0);

	return single_status;
}

struct status 
*libtwit_status_update(char *message)
{
	struct status *status_update;

	char *options[][2] = {
		{ "status", message }
	};

	if (libtwit_check_update_length(message) == LIBTWIT_MESSAGE_TOO_LONG) 
		return;
	else {
		status_update = parse_status_doc(POST_REQUEST, STATUS_URL, UPDATE, options, LENGTH(options));
		if (status_update != NULL)
			return status_update;
		else
			return NULL;
	}
}

struct status
*libtwit_get_recent_replies(char *optional_parameters[][2], int param_length)
{
	struct status *recent_replies = parse_status_doc(GET_REQUEST, STATUS_URL, RECENT_REPLIES, optional_parameters, param_length);

	return recent_replies;
}

struct status
*libtwit_destroy_status(int id)
{
	char destroy_status_url[SLENGTH];
	char id_char[SLENGTH];

	sprintf(id_char, "%i", id);
	sprintf(destroy_status_url, "%s%s.xml", DESTROY_STATUS, id_char);

	char *options[][2] = {
		{ "id", id_char }
	};

	struct status *destroy_status = parse_status_doc(POST_REQUEST, STATUS_URL, destroy_status_url, options, LENGTH(options));

	return destroy_status;
}
