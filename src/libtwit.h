#ifndef LIBTWIT_H
#define LIBTWIT_H

#include <libxml/parser.h>
#include <time.h>

#include "stack.h"

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

char *libtwit_twitter_username;
char *libtwit_twitter_password;

void display_tweets(struct tweet *starting_tweet);

struct tweet *parse_tweets(xmlNodePtr cur);

struct tweet *parse_user_timeline();

struct tweet *parse_friends_timeline();

int twitter_login(char *username, char *password);

#endif
