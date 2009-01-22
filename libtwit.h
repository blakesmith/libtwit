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
	char *created_at;
	int id;
	char *text;
	char *source;
	int truncated;
	int in_reply_to_status_id;
	int in_reply_to_user_id;
	int favorited;
	char *in_reply_to_screen_name;
	struct twitter_user user;
};

