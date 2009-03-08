/* libtwit (C Twitter library)
* (c) Blake Smith
* Project homepage: http://github.com/blakesmith/libtwit/tree/master

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef LIBTWIT_CORE_H
#define LIBTWIT_CORE_H
#include <libxml/parser.h>
#include <curl/curl.h>
#include <time.h>

#define FRIENDS_TIMELINE "friends_timeline.xml"
#define USER_TIMELINE "user_timeline.xml"
#define UPDATE "update.xml"
#define VERIFY_CREDENTIALS "verify_credentials.xml"
#define STATUS_URL "http://twitter.com/statuses/"
#define ACCOUNT_URL "http://twitter.com/account/"

#define SLENGTH 254

extern enum {
	/**
	 * Start our twitter return codes at 200 so we can use them from
	 * functions that may return a CURLcode as well as an internal 
	 * libtwit code.
	 */
	LIBTWIT_OK = 200,
	LIBTWIT_CREDENTIAL_ERROR,
	LIBTWIT_MESSAGE_TOO_LONG,
	LIBTWIT_TRANSMISSION_ERROR,
} libtwit_code;


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

extern CURL *libtwit_curl_handle;
extern CURLcode libtwit_curl_code;
extern char *libtwit_twitter_username;
extern char *libtwit_twitter_password;

void destroy_tweets(struct tweet *current);

void *libtwit_init();

void libtwit_deinit();

void display_tweets(struct tweet *starting_tweet);

struct tweet *parse_tweets(xmlNodePtr cur);

struct tweet *parse_tweet_doc();

int send_post_message(char *url, char *file, char *in_message);

int send_post_request(char *url, char *file, char *in_message);

int twitter_login(char *username, char *password);

static size_t xml_write_callback(void *ptr, size_t size, size_t nmemb, void *data);

static size_t empty_callback(void *ptr, size_t size, size_t nmemb, void *data);
#endif
