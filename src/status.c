#include "status.h"
#include "core.h"

#include <string.h>

int 
check_update_length(char *message)
{
	if (strlen(message) > 140)
		return 0;
	else
		return 1;
}

struct tweet 
*parse_friends_timeline()
{
	struct tweet *user_timeline = parse_tweet_doc(FRIENDS_TIMELINE);
	
	return user_timeline;
}

struct tweet 
*parse_user_timeline()
{
	struct tweet *user_timeline = parse_tweet_doc(USER_TIMELINE);
	
	return user_timeline;
}

int 
send_update(char *message)
{
	int success;

	if (check_update_length(message)) {
		success = send_post_update(STATUS_URL, UPDATE, message);
		if (success)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}
