#include "../src/status.h"
#include "test_app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tweet *parse_action(char *arg)
{
	struct tweet *tweets = NULL;

	if (strcmp(arg, "user") == 0)
	{
		tweets = parse_user_timeline();
	}
	else if (strcmp(arg, "friends") == 0)
	{
		tweets = parse_friends_timeline();
	}
	else
	{
		display_usage();
	}

	if (tweets)
		return tweets;
	else
		return NULL;
}

void display_usage()
{
	printf("Usage: test_app ACTION <twitter_login> <twitter_password>\n"
		"Valid actions:\n"			
		"\tuser\n"
		"\tfriends\n"
		);
	exit(0);
}

int main(int argc, char *argv[])
{
	struct tweet *tweets;

	libtwit_init();

	if (!(argc == 4))
	{
		display_usage();
	}
	twitter_login(argv[2], argv[3]);
	tweets = parse_action(argv[1]);
	if (tweets == NULL)
	{
		printf("Failed to retrieve timeline. Did you type your login information correctly?\n");
		exit(0);
	}
	display_tweets(tweets);

	libtwit_deinit();

	return 0;
}
