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

#include "test_app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libtwit/status.h>
#include <libtwit/output.h>

void parse_action(char *argv[])
{
	int success;

	if (strcmp(argv[1], "public") == 0)
		handle_tweets(libtwit_parse_public_timeline(NULL, 0));

	else if (strcmp(argv[1], "user") == 0)
	{
		if (libtwit_verify_credentials(argv[2], argv[3]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		handle_tweets(libtwit_parse_user_timeline(NULL, 0));
	}
	else if (strcmp(argv[1], "user_since") == 0)
	{
		if (libtwit_verify_credentials(argv[2], argv[3]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		char *options[][2] = {
			{ "since_id", "1301814112" }
		};

		handle_tweets(libtwit_parse_user_timeline(options, LENGTH(options)));
	}
	else if (strcmp(argv[1], "friends") == 0)
	{
		if (libtwit_verify_credentials(argv[2], argv[3]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		handle_tweets(libtwit_parse_friends_timeline(NULL, 0));
	}
	else if (strcmp(argv[1], "update") == 0)
	{
		if (libtwit_verify_credentials(argv[3], argv[4]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		handle_tweets(libtwit_status_update(argv[2]));
	}
	else if (strcmp(argv[1], "single") == 0)
	{
		if (libtwit_verify_credentials(argv[2], argv[3]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		handle_tweets(libtwit_parse_single_status(1416036771));
	}
	else
	{
		display_usage();
	}

}

void display_usage()
{
	printf("Usage: test_app ACTION <twitter_login> <twitter_password>\n"
		"Valid actions:\n"			
		"\tupdate <message>\n"
		"\tuser\n"
		"\tfriends\n"
		);
	exit(0);
}

void handle_tweets(struct status *first_tweet)
{
	if (first_tweet == NULL)
	{
		printf("Failed to retrieve timeline. Did you type your login information correctly?\n");
		exit(0);
	}
	else
		status_printf(first_tweet);
		destroy_statuses(first_tweet);
}

int main(int argc, char *argv[])
{
	struct status *tweets;

	libtwit_init();

	if ((argc < 4))
	{
		display_usage();
	}
	parse_action(argv);

	libtwit_deinit();

	return 0;
}
