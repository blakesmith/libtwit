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
#include <stdio.h>
#include <stdlib.h>

#include "core.h"

void 
status_printf(struct status *starting_tweet)
{
	struct status *i;

	if (starting_tweet == NULL)
		return;

	for (i = starting_tweet; i != NULL; i = i->next) {
		char created_char[SLENGTH];
		strftime(created_char, SLENGTH, "%a %b %d %H:%M:%S %Y", &(i->created_at));
		printf("created_at: %s\n"
			"id: %i\n"
			"text: %s\n"
			"source: %s\n"
			"truncated: %i\n"
			"in_reply_to_status_id: %i\n"
			"in_reply_to_user_id: %i\n"
			"favorited: %i\n"
			"in_reply_to_screen_name: %s\n" 
			"user: \n"
			"\tid: %i\n"
			"\tname: %s\n"
			"\tscreen_name: %s\n"
			"\tlocation: %s\n"
			"\tdescription: %s\n"
			"\tprofile_image_url: %s\n"
			"\turl: %s\n"
			"\tprotected: %i\n"
			"\tfollowers_count: %i\n"
			"===================================\n",
			
			created_char,
			i->id,
			i->text,
			i->source,
			i->truncated,
			i->in_reply_to_status_id,
			i->in_reply_to_user_id,
			i->favorited,
			i->in_reply_to_screen_name,
			i->user->id,
			i->user->name,
			i->user->screen_name,
			i->user->location,
			i->user->description,
			i->user->profile_image_url,
			i->user->url,
			i->user->prot,
			i->user->followers_count);
	}
}

void
basic_user_printf(struct basic_user *starting_user)
{
	struct basic_user *i;

	if (starting_user == NULL)
		return;

	for (i = starting_user; i != NULL; i = i->next) {
		char created_char[SLENGTH];
		strftime(created_char, SLENGTH, "%a %b %d %H:%M:%S %Y", &(i->status->created_at));
		printf("id: %i\n"
			"name: %s\n"
			"screen_name: %s\n"
			"location: %s\n"
			"description: %s\n"
			"profile_image_url: %s\n"
			"url: %s\n"
			"protected: %i\n"
			"followers_count: %i\n"
			"\tcreated_at: %s\n"
			"\tid: %i\n"
			"\ttext: %s\n"
			"\tsource: %s\n"
			"\ttruncated: %i\n"
			"\tin_reply_to_status_id: %i\n"
			"\tin_reply_to_user_id: %i\n"
			"\tfavorited: %i\n"
			"\tin_reply_to_screen_name: %s\n" 
			"\tuser: \n"
			"===================================\n",

			i->id,
			i->name,
			i->screen_name,
			i->location,
			i->description,
			i->profile_image_url,
			i->url,
			i->prot,
			i->followers_count,
			created_char,
			i->status->id,
			i->status->text,
			i->status->source,
			i->status->truncated,
			i->status->in_reply_to_status_id,
			i->status->in_reply_to_user_id,
			i->status->favorited,
			i->status->in_reply_to_screen_name,
			i->status->user);
	}
}
