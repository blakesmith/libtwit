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

#define LENGTH(x)	(sizeof(x) / sizeof(x[0]))

int 
check_update_length(char *message)
{
	if (strlen(message) > 140)
		return LIBTWIT_MESSAGE_TOO_LONG;
	else
		return LIBTWIT_OK;
}

struct tweet 
*parse_friends_timeline()
{
	struct tweet *user_timeline = parse_tweet_doc(FRIENDS_TIMELINE);
	
	return user_timeline;
}

struct tweet 
*parse_user_timeline()
{
	struct tweet *user_timeline = parse_tweet_doc(USER_TIMELINE);
	
	return user_timeline;
}

int 
send_update(char *message)
{
	int success;

	char *options[][2] = {
		{ "status", message }
	};

	if (check_update_length(message) == LIBTWIT_MESSAGE_TOO_LONG) 
		return LIBTWIT_MESSAGE_TOO_LONG;
	else {
		success = send_post_request(STATUS_URL, UPDATE, options, LENGTH(options));
		if (success == LIBTWIT_OK)
			return LIBTWIT_OK;
		else
			return LIBTWIT_TRANSMISSION_ERROR;
	}
}
