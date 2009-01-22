struct twitter_user
{
	int id;
	char *name;
	char *screen_name;
	char *location;
	char *description;
	char *profile_image_url;
	char *url;
	int prot;
	int follower_count;
};

struct tweet
{
	struct tweet *prev;
	struct tweet *next;
	xmlChar *created_at;
	int id;
	xmlChar *text;
	xmlChar *source;
	int truncated;
	int in_reply_to_status_id;
	int in_reply_to_user_id;
	int favorited;
	xmlChar *in_reply_to_screen_name;
	struct twitter_user *user;
};

