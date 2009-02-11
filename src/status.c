#include "status.h"
#include "core.h"

struct tweet *parse_friends_timeline()
{
	struct tweet *user_timeline = parse_tweet_doc(FRIENDS_TIMELINE);
	
	return user_timeline;
}

struct tweet *parse_user_timeline()
{
	struct tweet *user_timeline = parse_tweet_doc(USER_TIMELINE);
	
	return user_timeline;
}

int send_update(char *message)
{
	int success;

	success = send_post_update(STATUS_URL, UPDATE, message);
	if (success)
		return 1;
	else
		return 0;
}
