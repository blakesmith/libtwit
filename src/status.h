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
/**
 * statuses/public_timeline
 *
 * Returns the 20 most recent statuses from non-protected users who have set a custom user icon.  
 * Does not require authentication.  Note that the public timeline is cached for 60 seconds so 
 * requesting it more often than that is a waste of resources.
 */

struct status *libtwit_parse_friends_timeline(char *optional_parameters[][2], int param_length);
/**
 * statuses/friends_timeline
 *
 * Returns the 20 most recent statuses posted by the authenticating user and that user's friends. 
 * This is the equivalent of /home on the Web. 
 *
 * OPTIONAL PARAMETERS:
 * - since. Narrows the returned results to just those statuses created after the specified HTTP-formatted date, up to 24 hours old.
 * - since_id. Returns only statuses with an ID greater than (that is, more recent than) the specified ID.
 * - max_id. Returns only statuses with an ID less than (that is, older than) the specified ID.
 * - count. Specifies the number of statuses to retrieve. May not be greater than 200. 
 * - page.
 */

struct status *libtwit_parse_user_timeline(char *optional_parameters[][2], int param_length);
/**
 * statuses/user_timeline
 *
 * Returns the 20 most recent statuses posted from the authenticating user. It's also possible to 
 * request another user's timeline via the id parameter below. This is the equivalent of the Web 
 * /archive page for your own user, or the profile page for a third party.
 *
 * OPTIONAL PARAMETERS:
 * - id. Specifies the ID or screen name of the user for whom to return the user_timeline.
 * - user_id. Specfies the ID of the user for whom to return the user_timeline. Helpful for disambiguating when a valid user ID is also a valid screen name.
 * - screen_name. Returns only statuses with an ID greater than (that is, more recent than) the specified ID.
 * - max_id. Returns only statuses with an ID less than (that is, older than) the specified ID. 
 * - page. 
 * - since. Narrows the returned results to just those statuses created after the specified HTTP-formatted date, up to 24 hours old.
 */

struct status *libtwit_parse_single_status(int id);
/**
 * statuses/show
 * Returns a single status, specified by the id parameter below.  The status's author will be returned inline.
 *
 * REQUIRED PARAMETERS:
 * - id. The numerical ID of the status you're trying to retrieve.
 */

struct status *libtwit_status_update(char *message);
/**
 * statuses/update
 *
 * Updates the authenticating user's status.  Requires the status parameter specified below. A status update with text identical to the 
 * authenticating user's current status will be ignored.
 *
 * REQUIRED PARAMETERS:
 * - status. The text of your status update.  Be sure to URL encode as necessary.  Should not be more than 140 characters.
 *
 * OPTIONAL PARAMETERS:
 *
 * - in_reply_to_status_id. The ID of an existing status that the status to be posted is in reply to.  This implicitly sets the 
 *   in_reply_to_user_id attribute of the resulting status to the user ID of the message being replied to.  Invalid/missing status IDs will be ignored.
 */

struct status *libtwit_get_recent_replies(char *optional_parameters[][2], int param_length);
/**
 * statuses/replies
 * Returns the 20 most recent @replies (status updates prefixed with @username) for the authenticating user.
 *
 * OPTIONAL PARAMETERS:
 * - since_id. Returns only statuses with an ID greater than (that is, more recent than) the specified ID.
 * - max_id. Returns only statuses with an ID less than (that is, older than) the specified ID.
 * - since. Narrows the returned results to just those replies created after the specified HTTP-formatted date, up to 24 hours old.
 * - page. Retrieves the 20 next most recent replies.
 */

struct status *libtwit_destroy_status(int id);
/**
 * statuses/destroy
 * Destroys the status specified by the required ID parameter.  The authenticating user must be the author of the specified status.
 *
 * REQUIRED PARAMETERS:
 * - id. The ID of the status to destroy.
 */

#endif
