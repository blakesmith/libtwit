#include <libxml/parser.h>
#include <time.h>

#include "stack.h"

#define FRIENDS_TIMELINE "friends_timeline.xml"
#define USER_TIMELINE "user_timeline.xml"
#define UPDATE "update.xml"
#define STATUS_URL "http://twitter.com/statuses/"

#define SLENGTH 255


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
	struct tm created_at;
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

struct xml_memory
{
	char *memory;
	size_t size;
};

char *libtwit_twitter_username;
char *libtwit_twitter_password;

void display_tweets(struct tweet *starting_tweet);

struct tweet *parse_tweets(xmlNodePtr cur);

struct tweet *parse_tweet_doc();

int send_post_update(char *url, char *file, char *in_message);

int twitter_login(char *username, char *password);

static size_t
xml_write_callback(void *ptr, size_t size, size_t nmemb, void *data);
