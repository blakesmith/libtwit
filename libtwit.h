struct twitter_user
{
	int id;
	xmlChar *name;
	xmlChar *screen_name;
	xmlChar *location;
	xmlChar *description;
	xmlChar *profile_image_url;
	xmlChar *url;
	int prot;
	int followers_count;
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

