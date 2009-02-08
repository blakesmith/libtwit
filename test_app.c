#include "libtwit.h"

int main(int argc, char *argv[])
{
	struct tweet *tweets;

	libtwit_init();

	twitter_login(argv[1], argv[2]);
	tweets = parse_user_timeline();
	display_tweets(tweets);

	libtwit_deinit();

	return 0;
}
