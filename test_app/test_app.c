#include "../src/status.h"
#include "test_app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_action(char *arg)
{
	int success;

	if (strcmp(arg, "user") == 0)
	{
		handle_tweets(parse_user_timeline());
	}
	else if (strcmp(arg, "friends") == 0)
	{
		handle_tweets(parse_friends_timeline());
	}
	else if (strcmp(arg, "update") == 0)
	{
		success = send_update("Testing libtwit.");
		if (success)
			printf("Update sent.\n");
		else
			printf("Failed to post update.\n");
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
		"\tuser\n"
		"\tfriends\n"
		);
	exit(0);
}

void handle_tweets(struct tweet *first_tweet)
{
	if (first_tweet == NULL)
	{
		printf("Failed to retrieve timeline. Did you type your login information correctly?\n");
		exit(0);
	}
	else
		display_tweets(first_tweet);
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
	parse_action(argv[1]);

	libtwit_deinit();

	return 0;
}
