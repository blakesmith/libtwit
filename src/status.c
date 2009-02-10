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
