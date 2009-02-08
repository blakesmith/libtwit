#include "src/libtwit.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct tweet *tweets;

	libtwit_init();

	if (!(argc == 3))
	{
		printf("Usage: test_app <twitter_login> <twitter_password>\n");
		exit(0);
	}
	twitter_login(argv[1], argv[2]);
	tweets = parse_user_timeline();
	if (tweets == NULL)
	{
		printf("Failed to retrieve user timeline. Did you type your login information correctly?\n");
		exit(0);
	}
	display_tweets(tweets);

	libtwit_deinit();

	return 0;
}
