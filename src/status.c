#include "status.h"
#include "core.h"

#include <string.h>

int 
check_update_length(char *message)
{
	if (strlen(message) > 140)
		return LIBTWIT_MESSAGE_TOO_LONG;
	else
		return LIBTWIT_OK;
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

	if (check_update_length(message) == LIBTWIT_MESSAGE_TOO_LONG) 
		return LIBTWIT_MESSAGE_TOO_LONG;
	else {
		success = send_post_update(STATUS_URL, UPDATE, message);
		if (success == LIBTWIT_OK)
			return LIBTWIT_OK;
		else
			return LIBTWIT_TRANSMISSION_ERROR;
	}
}
