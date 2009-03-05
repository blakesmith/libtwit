#include "../src/status.h"
#include "test_app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_action(char *argv[])
{
	int success;

	if (strcmp(argv[1], "user") == 0)
	{
		if (twitter_login(argv[2], argv[3]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		handle_tweets(parse_user_timeline());
	}
	else if (strcmp(argv[1], "friends") == 0)
	{
		if (!twitter_login(argv[2], argv[3]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		handle_tweets(parse_friends_timeline());
	}
	else if (strcmp(argv[1], "update") == 0)
	{
		if (!twitter_login(argv[3], argv[4]) == LIBTWIT_CREDENTIAL_ERROR) {
			printf("Error logging in. Are your credentials correct?\n");
			libtwit_deinit();
			exit(0);
		}
		success = send_update(argv[2]);
		if (success == LIBTWIT_MESSAGE_TOO_LONG) {
			printf("Message exceeds 140 characters, please shorten it.\n");
			libtwit_deinit();
			exit(0);
		}
		else if (success == LIBTWIT_TRANSMISSION_ERROR) {
			printf("libcurl died during transmission for some reason.\n");
			libtwit_deinit();
			exit(0);
		}
		else if (success == LIBTWIT_OK)
			printf("Update sent->\"%s\"\n", argv[2]);
		else {
			printf("Update died for an unknown reason.");
			libtwit_deinit();
			exit(0);
		}
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

void handle_tweets(struct tweet *first_tweet)
{
	if (first_tweet == NULL)
	{
		printf("Failed to retrieve timeline. Did you type your login information correctly?\n");
		exit(0);
	}
	else
		display_tweets(first_tweet);
		destroy_tweets(first_tweet);
}

int main(int argc, char *argv[])
{
	struct tweet *tweets;

	libtwit_init();

	if ((argc < 4))
	{
		display_usage();
	}
	parse_action(argv);

	libtwit_deinit();

	return 0;
}
